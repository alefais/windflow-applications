/**
 *  @file    main.cpp
 *  @author  Alessandra Fais
 *  @date    14/05/2019
 *
 *  @brief main of the SpikeDetection application
 */

#include <iostream>
#include <string>
#include <vector>
#include <regex>

#include <ff/ff.hpp>
#include <windflow.hpp>

#include "../../includes/util/cli_util.hpp"
#include "../../includes/util/constants.hpp"
#include "../../includes/util/tuple.hpp"
#include "../../includes/nodes/source.hpp"

using namespace std;

/// type of the input records: < date_value, time_value, epoch_value, device_id_value, temp_value, humid_value, light_value, voltage_value>
using record_t = tuple<string, string, int, int, double, double, double, double>;

/// defines information contained in each record in the dataset
typedef enum { DATE_FIELD, TIME_FIELD, EPOCH_FIELD, DEVICE_ID_FIELD, TEMP_FIELD, HUMID_FIELD, LIGHT_FIELD, VOLT_FIELD } record_field;

vector<record_t> parsed_file;               // contains data extracted from the input file
vector<tuple_t> dataset;                    // contains all the tuples in memory
unordered_map<size_t, uint64_t> key_occ;    // contains the number of occurrences of each key device_id

/**
 *  @brief Parse the input file
 *
 *  The file is parsed and saved in memory.
 *
 *  @param file_path the path of the input dataset file
 */
void parse_dataset(const string& file_path) {
    ifstream file(file_path);
    if (file.is_open()) {
        size_t all_records = 0;         // counter of all records (dataset line) read
        size_t incomplete_records = 0;  // counter of the incomplete records

        string line;
        while (getline(file, line) && all_records < 50) {
            // process file line
            int token_count = 0;
            vector<string> tokens;

            regex rgx("\\s+"); // regex quantifier (matches one or many whitespaces)
            sregex_token_iterator iter(line.begin(), line.end(), rgx, -1);
            sregex_token_iterator end;

            while (iter != end) {
                tokens.push_back(*iter);
                token_count++;
                iter++;
            }

            // a record is valid if it contains at least 8 values (one for each field of interest)
            if (token_count >= 8) {
                // save parsed file
                record_t r(tokens.at(DATE_FIELD),
                           tokens.at(TIME_FIELD),
                           atoi(tokens.at(EPOCH_FIELD).c_str()),
                           atoi(tokens.at(DEVICE_ID_FIELD).c_str()),
                           atof(tokens.at(TEMP_FIELD).c_str()),
                           atof(tokens.at(HUMID_FIELD).c_str()),
                           atof(tokens.at(LIGHT_FIELD).c_str()),
                           atof(tokens.at(VOLT_FIELD).c_str()));
                parsed_file.push_back(r);

                // insert the key in the map (if it is not present)
                if (key_occ.find(get<DEVICE_ID_FIELD>(r)) == key_occ.end()) {
                    key_occ.insert(make_pair(get<DEVICE_ID_FIELD>(r), 0));
                }

            } else
                incomplete_records++;

            all_records++;
        }
        file.close();

        /*cout << "[main] parsed " << all_records << " records ("
             << incomplete_records << " incomplete, "
             << all_records - incomplete_records << " valid)" << endl;
        for (auto r : parsed_file)
            cout << get<DEVICE_ID_FIELD>(r) << " - "
                 << get<TEMP_FIELD>(r) << " - "
                 << get<HUMID_FIELD>(r) << " - "
                 << get<LIGHT_FIELD>(r) << " - "
                 << get<VOLT_FIELD>(r) << endl;*/
    }
}

/**
 *  @brief Process parsed data and create all the tuples
 *
 *  The created tuples are maintained in memory. The source node will generate the stream by
 *  reading all the tuples from main memory.
 */
void create_tuples() {
    for (int next_tuple_idx = 0; next_tuple_idx < parsed_file.size(); next_tuple_idx++) {
        // create tuple
        auto record = parsed_file.at(next_tuple_idx);
        tuple_t t;
        if (_field == TEMPERATURE)
            t.property_value = get<TEMP_FIELD>(record);
        else if (_field == HUMIDITY)
            t.property_value = get<HUMID_FIELD>(record);
        else if (_field == LIGHT)
            t.property_value = get<LIGHT_FIELD>(record);
        else if (_field == VOLTAGE)
            t.property_value = get<VOLT_FIELD>(record);
        t.incremental_average = 0;
        t.key = get<DEVICE_ID_FIELD>(record);
        t.id = (key_occ.find(get<DEVICE_ID_FIELD>(record)))->second++;
        t.ts = 0L;
        dataset.insert(dataset.end(), t);
    }
}

int main(int argc, char* argv[]) {
    /// parse arguments from command line
    int option = 0;
    int index = 0;

    string file_path;
    size_t source_par_deg = 0;
    size_t average_par_deg = 0;
    size_t detector_par_deg = 0;
    size_t sink_par_deg = 0;
    int rate = 0;

    /* Program options:
     * - case (argc == 13):
     * specified parameters are --file, --nsource, --naverage, --ndetector, --nsink, --rate (and their arguments)
     * - case (argc == 7):
     * specified parameters are --file, --pardeg, --rate (and their arguments)
     * - case (argc == 2):
     * specified parameter is --help (with no argument)
     */
    opterr = 1;     // turn on/off getopt error messages
    if (argc == 13) {
        while ((option = getopt_long(argc, argv, "f:s:a:d:e:r:", long_opts, &index)) != -1) {
            switch (option) {
                case 'f': file_path = optarg;
                    break;
                case 's': source_par_deg = atoi(optarg);
                    break;
                case 'a': average_par_deg = atoi(optarg);
                    break;
                case 'd': detector_par_deg = atoi(optarg);
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
    } else if (argc == 7) {
        while ((option = getopt_long(argc, argv, "f:n:r:", long_opts, &index)) != -1) {
            switch (option) {
                case 'f': file_path = optarg;
                    break;
                case 'n':
                    source_par_deg = atoi(optarg);
                    average_par_deg = atoi(optarg);
                    detector_par_deg = atoi(optarg);
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

    /// data pre-processing
    parse_dataset(file_path);
    create_tuples();
    cout << "[main] dataset size: " << dataset.size() << endl;
    /*for (auto t : dataset)
        cout << t.property_value << " - "
             << t.incremental_average << " - "
             << t.key << " - "
             << t.id << " - "
             << t.ts << endl;*/

    /// application starting time
    unsigned long app_start_time = current_time_usecs();

    /// create the nodes
    Source_Functor source_functor(dataset, rate, app_start_time);
    Source source = Source_Builder(source_functor)
            .withParallelism(source_par_deg)
            .withName(source_name)
            .build();

    /// print application description
    print_app_descr(file_path, source_par_deg, average_par_deg, detector_par_deg, sink_par_deg, rate);

    /// create the multi pipe
    MultiPipe topology(topology_name);
    topology.add_source(source);
    if (topology.run_and_wait_end() < 0)
        cerr << app_error << endl;
    else
        cout << app_termination << topology.cardinality() << endl;

    return 0;
}