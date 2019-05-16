/**
 *  @file    main_light_source.cpp
 *  @author  Alessandra Fais
 *  @date    16/05/2019
 *
 *  @brief main of the FraudDetection application
 *
 *  The source used in this version generates the stream by
 *  reading the tuples from memory.
 */

#include <iostream>
#include <string>
#include <vector>
#include <ff/ff.hpp>
#include <windflow.hpp>

#include "../../includes/util/cli_util.hpp"
#include "../../includes/util/tuple.hpp"
#include "../../includes/nodes/light_source.hpp"
#include "../../includes/nodes/predictor.hpp"
#include "../../includes/nodes/sink.hpp"

using namespace std;
using count_key_t = pair<size_t, uint64_t>;
using key_map_t = unordered_map<string, count_key_t>;

key_map_t entity_key_map;                   // contains a mapping between string keys and integer keys for each entity_id
size_t entity_unique_key = 0;               // unique integer key
vector<pair<string, string>> parsed_file;   // contains strings extracted from the input file
vector<tuple_t> dataset;                    // contains all the tuples in memory

/**
 *  @brief Map keys and parse the input file
 *
 *  This method assigns to each string key entity_id a unique integer key (required by the current
 *  implementation of WindFlow). Moreover, the file is parsed and saved in memory.
 *
 *  @param file_path the path of the input dataset file
 *  @param split_regex the regular expression used to split the lines of the file
 *         (e.g. for a file input.csv the regular expression to be used is ",")
 */
void map_and_parse_dataset(const string& file_path, const string& split_regex) {
    ifstream file(file_path);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            string entity_id = line.substr(0, line.find(split_regex));
            string record = line.substr(line.find(split_regex) + 1, line.size());

            // map keys
            if (entity_key_map.find(entity_id) == entity_key_map.end()) { // the key is not present in the hash map
                entity_key_map.insert(make_pair(entity_id, make_pair(entity_unique_key, 0)));
                entity_unique_key++;
            }

            // save parsed file
            parsed_file.insert(parsed_file.end(), make_pair(entity_id, record));
        }
        file.close();
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
        auto tuple_content = parsed_file.at(next_tuple_idx);
        tuple_t t;
        t.entity_id = tuple_content.first;
        t.record = tuple_content.second;
        t.key = (entity_key_map.find(t.entity_id)->second).first;
        t.id = ((entity_key_map.find(t.entity_id))->second).second++;
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
    size_t predictor_par_deg = 0;
    size_t sink_par_deg = 0;
    int rate = 0;

    /* Program options:
     * - case (argc == 11):
     * specified parameters are --file, --nsource, --npredictor, --nsink, --rate (and their arguments)
     * - case (argc == 7):
     * specified parameters are --file, --pardeg, --rate (and their arguments)
     * - case (argc == 2):
     * specified parameter is --help (with no argument)
     */
    opterr = 1;     // turn on/off getopt error messages
    if (argc == 11) {
        while ((option = getopt_long(argc, argv, "f:s:p:e:r:", long_opts, &index)) != -1) {
            switch (option) {
                case 'f': file_path = optarg;
                    break;
                case 's': source_par_deg = atoi(optarg);
                    break;
                case 'p': predictor_par_deg = atoi(optarg);
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
                    predictor_par_deg = atoi(optarg);
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
    map_and_parse_dataset(file_path, ",");
    create_tuples();

    /// application starting time
    unsigned long app_start_time = current_time_usecs();

    /// create the nodes
    Source_Functor source_functor(dataset, rate, app_start_time);
    Source source = Source_Builder(source_functor)
            .withParallelism(source_par_deg)
            .withName(light_source_name)
            .build();

    Predictor_Functor predictor_functor;
    FlatMap predictor = FlatMap_Builder(predictor_functor)
            .withParallelism(predictor_par_deg)
            .withName(predictor_name)
            .enable_KeyBy()
            .build();

    Sink_Functor sink_functor(rate, app_start_time);
    Sink sink = Sink_Builder(sink_functor)
            .withParallelism(sink_par_deg)
            .withName(sink_name)
            .build();

    /// print application description
    print_app_descr(file_path, source_par_deg, predictor_par_deg, sink_par_deg, rate);

    /// create the multi pipe
    MultiPipe topology(topology_name);
    topology.add_source(source);
    topology.add(predictor);
    topology.add_sink(sink);
    if (topology.run_and_wait_end() < 0)
        cerr << app_error << endl;
    else
        cout << app_termination << topology.getNumThreads() << endl;

    return 0;
}