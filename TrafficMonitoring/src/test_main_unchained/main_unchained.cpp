/**
 *  @file    main_unchained.cpp
 *  @author  Alessandra Fais
 *  @date    18/07/2019
 *
 *  @brief main of the SpikeDetection application
 *
 *  Note on the datasets used:
 *
 *  * Beijing vehicle-trace dataset is used freely, with the following acknowledgement:
 *    “This code was obtained from research conducted by the University of Southern
 *    California’s Autonomous Networks Research Group, http://anrg.usc.edu“.
 *    Format of the Beijing dataset:
 *    < vehicleID, nID, date-time, latitude, longitude, speed, bearing (direction) >
 *
 *  * GPS data about buses across Dublin City are retrieved from Dublin City Council'traffic control.
 *    See https://data.gov.ie/dataset/dublin-bus-gps-sample-data-from-dublin-city-council-insight-project
 *    Format of the Dublin dataset:
 *    < timestamp, lineID, direction, journeyPatternID, timeFrame, vehicleJourneyID, busOperator, congestion,
 *      longitude, latitude, delay, blockID, vehicleID, stopID, atStop >
 */

#include <iostream>
#include <string>
#include <vector>
#include <regex>

#include <ff/ff.hpp>
#include <windflow.hpp>

#include "../../includes/util/cli_util.hpp"
#include "../../includes/util/atomic_double.hpp"
#include "../../includes/util/constants.hpp"
#include "../../includes/util/tuple.hpp"
#include "../../includes/nodes/source.hpp"
#include "../../includes/nodes/map_matcher.hpp"
#include "../../includes/nodes/speed_calculator.hpp"
#include "../../includes/nodes/sink.hpp"

using namespace std;

// Type of Beijing input records:
// < vehicle_ID_value, n_ID_value, date_value, latitude_value, longitude_value, speed_value, direction_value >
using beijing_record_t = tuple<int, int, string, double, double, double, int>;

// Type of Dublin input records:
// < timestamp_value, line_ID_value, direction_value, journey_pattern_ID_value, time_frame_value, vehicle_journey_ID_value, operator_value,
// congestion_value, longitude_value, latitude_value, delay_value, block_ID_value, bus_ID_value, stop_ID_value, at_stop_value >
using dublin_record_t = tuple<long, int, int, string, string, int, string, int, double, double, int, int, int, int, int>;

vector<beijing_record_t> beijing_parsed_file;           // contains data extracted from the Beijing input file
vector<dublin_record_t> dublin_parsed_file;             // contains data extracted from the Dublin input file
vector<tuple_t> dataset;                                // contains all the tuples in memory
unordered_map<size_t, uint64_t> key_occ;                // contains the number of occurrences of each key vehicle_id
Road_Grid_List road_grid_list;                          // contains data extracted from the city shapefile
atomic<long> sent_tuples;                               // total number of tuples sent by all the sources
Atomic_Double average_latency_sum;                      // sum of the average latency values measured in each of the sink's replicas
atomic<int> sink_zero_processed;                        // number of sink's replicas that processed zero tuples

/**
 *  @brief Parse the input file.
 *
 *  The file is parsed and saved in memory.
 *  @param file_path the path of the input dataset file
 */
void parse_dataset(const string& file_path) {
    ifstream file(file_path);
    if (file.is_open()) {
        size_t all_records = 0;         // counter of all records (dataset line) read
        size_t incomplete_records = 0;  // counter of the incomplete records

        string line;
        while (getline(file, line)) {
            // process file line
            int token_count = 0;
            vector<string> tokens;

            size_t last = 0;
            size_t next = 0;
            while ((next = line.find(',', last)) != string::npos) {
                tokens.push_back(line.substr(last, next - last));
                last = next + 1;
                token_count++;
            }
            tokens.push_back(line.substr(last));
            token_count++;

            // A record is valid if it contains at least 7 values (one for each field of interest)
            // in the case in which the application analyzes data coming from Beijing taxi-traces.
            if (_monitored_city == BEIJING) {
                if (token_count >= 7) {
                    // save parsed file
                    beijing_record_t r(atoi(tokens.at(TAXI_ID_FIELD).c_str()),
                                       atoi(tokens.at(NID_FIELD).c_str()),
                                       tokens.at(DATE_FIELD),
                                       atof(tokens.at(TAXI_LATITUDE_FIELD).c_str()),
                                       atof(tokens.at(TAXI_LONGITUDE_FIELD).c_str()),
                                       atof(tokens.at(TAXI_SPEED_FIELD).c_str()),
                                       atoi(tokens.at(TAXI_DIRECTION_FIELD).c_str()));
                    beijing_parsed_file.push_back(r);

                    // insert the key device_id in the map (if it is not present)
                    if (key_occ.find(get<TAXI_ID_FIELD>(r)) == key_occ.end()) {
                        key_occ.insert(make_pair(get<TAXI_ID_FIELD>(r), 0));
                    }
                } else
                    incomplete_records++;
            } else if (_monitored_city == DUBLIN) {
                // A record is valid if it contains at least 15 values (one for each field of interest)
                // in the case in which the application analyzes data coming from Dublin bus-traces.
                if (token_count >= 15) {
                    // save parsed file
                    dublin_record_t r(atol(tokens.at(TIMESTAMP_FIELD).c_str()),
                                      atoi(tokens.at(LINE_ID_FIELD).c_str()),
                                      atoi(tokens.at(BUS_DIRECTION_FIELD).c_str()),
                                      tokens.at(JOURNEY_PATTERN_ID_FIELD),
                                      tokens.at(TIME_FRAME_FIELD),
                                      atoi(tokens.at(VEHICLE_JOURNEY_ID_FIELD).c_str()),
                                      tokens.at(OPERATOR_FIELD),
                                      atoi(tokens.at(CONGESTION_FIELD).c_str()),
                                      atof(tokens.at(BUS_LONGITUDE_FIELD).c_str()),
                                      atof(tokens.at(BUS_LATITUDE_FIELD).c_str()),
                                      atoi(tokens.at(DELAY_FIELD).c_str()),
                                      atoi(tokens.at(BLOCK_ID_FIELD).c_str()),
                                      atoi(tokens.at(BUS_ID_FIELD).c_str()),
                                      atoi(tokens.at(STOP_ID_FIELD).c_str()),
                                      atoi(tokens.at(AT_STOP_ID_FIELD).c_str()));
                    dublin_parsed_file.push_back(r);

                    // insert the key device_id in the map (if it is not present)
                    if (key_occ.find(get<BUS_ID_FIELD>(r)) == key_occ.end()) {
                        key_occ.insert(make_pair(get<BUS_ID_FIELD>(r), 0));
                    }
                } else
                    incomplete_records++;
            }

            all_records++;
        }
        file.close();

        //if (_monitored_city == BEIJING) print_taxi_parsing_info(beijing_parsed_file, all_records, incomplete_records);
        //else if (_monitored_city == DUBLIN) print_bus_parsing_info(dublin_parsed_file, all_records, incomplete_records);
    }
}

/**
 *  @brief Process parsed data and create all the tuples.
 *
 *  The created tuples are maintained in memory. The source node will generate the stream by
 *  reading all the tuples from main memory.
 */
void create_tuples() {
    if (_monitored_city == BEIJING) {
        for (int next_tuple_idx = 0; next_tuple_idx < beijing_parsed_file.size(); next_tuple_idx++) {
            // create tuple
            beijing_record_t record = beijing_parsed_file.at(next_tuple_idx);
            tuple_t t;
            t.latitude = get<TAXI_LATITUDE_FIELD>(record);
            t.longitude = get<TAXI_LONGITUDE_FIELD>(record);
            t.speed = get<TAXI_SPEED_FIELD>(record);
            t.direction = get<TAXI_DIRECTION_FIELD>(record);
            t.key = get<TAXI_ID_FIELD>(record);
            t.id = (key_occ.find(get<TAXI_ID_FIELD>(record)))->second++;
            t.ts = 0L;
            dataset.insert(dataset.end(), t);
        }
    } else if (_monitored_city == DUBLIN) {
        for (int next_tuple_idx = 0; next_tuple_idx < dublin_parsed_file.size(); next_tuple_idx++) {
            // create tuple
            dublin_record_t record = dublin_parsed_file.at(next_tuple_idx);
            tuple_t t;
            t.latitude = get<BUS_LATITUDE_FIELD>(record);
            t.longitude = get<BUS_LONGITUDE_FIELD>(record);
            t.speed = 0.0; // speed values are not present in the used dataset
            t.direction = get<BUS_DIRECTION_FIELD>(record);
            t.key = get<BUS_ID_FIELD>(record);
            t.id = (key_occ.find(get<BUS_ID_FIELD>(record)))->second++;
            t.ts = 0L;
            dataset.insert(dataset.end(), t);
        }
    }
}

/**
 *  @brief Parse the shapefile and create a the Road_Grid_List data structure.
 *
 *  The data structure containing the processed information about the roads of the city
 *  is passed to the MapMatcher node and use to implement the map matching logic.
 */
void read_shapefile() {
    string shapefile_path = (_monitored_city == DUBLIN) ? _dublin_shapefile : _beijing_shapefile;
    if (road_grid_list.read_shapefile(shapefile_path) == -1)
        __throw_invalid_argument("Failed reading shapefile");
}

int main(int argc, char* argv[]) {
    /// parse arguments from command line
    int option = 0;
    int index = 0;

    string file_path;
    size_t source_par_deg = 0;
    size_t matcher_par_deg = 0;
    size_t calculator_par_deg = 0;
    size_t sink_par_deg = 0;
    int rate = 0;
    sent_tuples = 0;
    sink_zero_processed = 0;

    /* Program options:
     *
     * - case (argc == 11):
     * specified parameters are --nsource, --nmatcher, --ncalculator, --nsink, --rate (and their arguments)
     * NB: input dataset is selected depending on the value of _monitored_city parameter; both dataset file path and
     *     _monitored_city values are defined in constants.hpp
     *
     * - case (argc == 5):
     * specified parameters are --pardeg, --rate (and their arguments)
     * NB: input dataset is selected depending on the value of _monitored_city parameter; both dataset file path and
     *     _monitored_city values are defined in constants.hpp
     *
     * - case (argc == 2):
     * specified parameter is --help (with no argument)
     */
    opterr = 1;     // turn on/off getopt error messages
    if (argc == 11) {
        file_path = (_monitored_city == DUBLIN) ? _dublin_input_file : _beijing_input_file;
        while ((option = getopt_long(argc, argv, "s:m:c:e:r:", long_opts, &index)) != -1) {
            switch (option) {
                case 's': source_par_deg = atoi(optarg);
                    break;
                case 'm': matcher_par_deg = atoi(optarg);
                    break;
                case 'c': calculator_par_deg = atoi(optarg);
                    break;
                case 'e': sink_par_deg = atoi(optarg);
                    break;
                case 'r': rate = atoi(optarg);
                    break;
                default:
                    print_help(argv[0]);
                    exit(EXIT_SUCCESS);
            }
        }
    } else if (argc == 5) {
        while ((option = getopt_long(argc, argv, "n:r:", long_opts, &index)) != -1) {
            file_path = (_monitored_city == DUBLIN) ? _dublin_input_file : _beijing_input_file;
            switch (option) {
                case 'n':
                    source_par_deg = atoi(optarg);
                    matcher_par_deg = atoi(optarg);
                    calculator_par_deg = atoi(optarg);
                    sink_par_deg = atoi(optarg);
                    break;
                case 'r': rate = atoi(optarg);
                    break;
                default:
                    print_help(argv[0]);
                    exit(EXIT_SUCCESS);
            }
        }
    } else if (argc == 2) {
        while ((getopt_long(argc, argv, "h", long_opts, &index)) != -1) {
            print_help(argv[0]);
            exit(EXIT_SUCCESS);
        }
    } else {
        print_help(argv[0]);
        exit(EXIT_SUCCESS);
    }

    /// print application description
    print_app_descr(file_path, source_par_deg, matcher_par_deg, calculator_par_deg, sink_par_deg, rate);

    /// data pre-processing
    parse_dataset(file_path);
    create_tuples();
    read_shapefile();
    //print_dataset(dataset);

    /// application starting time
    unsigned long app_start_time = current_time_usecs();

    /// create the nodes
    Source_Functor source_functor(dataset, rate, app_start_time);
    Source source = Source_Builder(source_functor)
            .withParallelism(source_par_deg)
            .withName(source_name)
            .build();

    Map_Matcher_Functor map_match_functor(road_grid_list, app_start_time);
    FlatMap map_matcher = FlatMap_Builder(map_match_functor)
            .withParallelism(matcher_par_deg)
            .withName(map_match_name)
            .build();

    Speed_Calculator_Functor speed_calc_functor(app_start_time);
    Map speed_calculator = Map_Builder(speed_calc_functor)
            .withParallelism(calculator_par_deg)
            .withName(speed_calc_name)
            .enable_KeyBy()
            .build();

    Sink_Functor sink_functor(rate, app_start_time);
    Sink sink = Sink_Builder(sink_functor)
            .withParallelism(sink_par_deg)
            .withName(sink_name)
            .build();

    /// create the multi pipe
    MultiPipe topology(topology_name);
    topology.add_source(source);
    topology.add(map_matcher);
    topology.add(speed_calculator);
    topology.add_sink(sink);

    /// evaluate topology execution time
    volatile unsigned long start_time_main_usecs = current_time_usecs();
    if (topology.run_and_wait_end() < 0)
        cerr << app_error << endl;
    else
        cout << app_termination << topology.getNumThreads() << endl;
    volatile unsigned long end_time_main_usecs = current_time_usecs();
    double elapsed_time_seconds = (end_time_main_usecs - start_time_main_usecs) / (1000000.0);

    /// evaluate average latency value (average time required by the tuples to traverse the whole system)
    double tot_average_latency = average_latency_sum.get() / (sink_par_deg - sink_zero_processed);

    /// print application results summary (run with FF_BOUNDED_BUFFER set)
    print_summary(sent_tuples, elapsed_time_seconds, tot_average_latency);

    return 0;
}