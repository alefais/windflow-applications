/**
 *  @file    main.cpp
 *  @author  Alessandra Fais
 *  @date    14/07/2019
 *
 *  @brief main of the WordCount application
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
#include "../../includes/util/result.hpp"
#include "../../includes/nodes/source.hpp"
#include "../../includes/nodes/splitter.hpp"
#include "../../includes/nodes/counter.hpp"
#include "../../includes/nodes/sink.hpp"

using namespace std;

vector<tuple_t> dataset;                    // contains all the input tuples in memory
Atomic_Double total_MB;                     // total number of MB processed by the system
atomic<long> total_words;                   // total number of words processed by the system
Atomic_Double average_latency_sum;          // sum of the average latency values measured in each of the sink's replicas
atomic<int> sink_zero_processed;            // number of sink's replicas that processed zero tuples

/**
 *  @brief Parse the input file and create all the tuples
 *
 *  The created tuples are maintained in memory. The source node will generate the stream by
 *  reading all the tuples from main memory.
 *
 *  @param file_path the path of the input dataset file
 */
void parse_dataset_and_create_tuples(const string& file_path) {
    ifstream file(file_path);
    if (file.is_open()) {
        size_t all_records = 0;         // counter of all records (dataset line) read

        string line;
        while (getline(file, line)) {
            // process file line
            if (!line.empty()) {
                tuple_t t(line, all_records, 0, 0);
                all_records++;
                dataset.push_back(t);
            }
        }
        file.close();
    }
}

int main(int argc, char* argv[]) {
    /// parse arguments from command line
    int option = 0;
    int index = 0;

    string file_path;
    size_t source_par_deg = 0;
    size_t splitter_par_deg = 0;
    size_t counter_par_deg = 0;
    size_t sink_par_deg = 0;
    int rate = 0;
    sink_zero_processed = 0;

    /* Program options:
     *
     * - case (argc == 11):
     * specified parameters are --nsource, --nsplitter, --ncounter, --nsink, --rate (and their arguments)
     * NB: input dataset path is defined in util/constants.hpp
     *
     * - case (argc == 5):
     * specified parameters are --pardeg, --rate (and their arguments)
     * NB: input dataset path is defined in util/constants.hpp
     *
     * - case (argc == 2):
     * specified parameter is --help (with no argument)
     */
    opterr = 1;     // turn on/off getopt error messages
    if (argc == 11) {
        file_path = _input_file;
        while ((option = getopt_long(argc, argv, "s:p:c:e:r:", long_opts, &index)) != -1) {
            switch (option) {
                case 's': source_par_deg = atoi(optarg);
                    break;
                case 'p': splitter_par_deg = atoi(optarg);
                    break;
                case 'c': counter_par_deg = atoi(optarg);
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
            file_path = _input_file;
            switch (option) {
                case 'n':
                    source_par_deg = atoi(optarg);
                    splitter_par_deg = atoi(optarg);
                    counter_par_deg = atoi(optarg);
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
    print_app_descr(file_path, source_par_deg, splitter_par_deg, counter_par_deg, sink_par_deg, rate);

    /// data pre-processing
    parse_dataset_and_create_tuples(file_path);
    //print_dataset(dataset);

    /// application starting time
    unsigned long app_start_time = current_time_usecs();

    /// create the nodes
    Source_Functor source_functor(dataset, rate, app_start_time);
    Source source = Source_Builder(source_functor)
            .withParallelism(source_par_deg)
            .withName(source_name)
            .build();

    Splitter_Functor splitter_functor(app_start_time);
    FlatMap splitter = FlatMap_Builder(splitter_functor)
            .withParallelism(splitter_par_deg)
            .withName(splitter_name)
            .build();

    Counter_Functor counter_functor(app_start_time);
    Accumulator counter = Accumulator_Builder(counter_functor)
            .withParallelism(counter_par_deg)
            .withName(counter_name)
            .withInitialValue(result_t())
            .build();

    Sink_Functor sink_functor(rate, app_start_time);
    Sink sink = Sink_Builder(sink_functor)
            .withParallelism(sink_par_deg)
            .withName(sink_name)
            .build();

    /// create the multi pipe
    MultiPipe topology(topology_name);
    topology.add_source(source);   // in order to exploit chaining, source and splitter must have the same parallelism degree
    topology.chain(splitter);
    topology.add(counter);         // in order to exploit chaining, counter and sink must have the same parallelism degree
    topology.chain_sink(sink);

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
    print_summary(total_MB.get(), total_words, elapsed_time_seconds, tot_average_latency);

    return 0;
}