/**
 *  @file    main_heavy_source.cpp
 *  @author  Alessandra Fais
 *  @date    03/06/2019
 *
 *  @brief main of the FraudDetection application
 *
 *  The source used in this version maps and parses the dataset
 *  input file and then starts generating the stream. In this
 *  case we're paying the overhead of doing some pre-processing
 *  on the dataset in the source node.
 */

#include <iostream>
#include <string>
#include <ff/ff.hpp>
#include <windflow.hpp>

#include "../../includes/util/cli_util.hpp"
#include "../../includes/nodes/heavy_source.hpp"
#include "../../includes/nodes/predictor.hpp"
#include "../../includes/nodes/sink.hpp"

using namespace std;

atomic<long> sent_tuples;                   // total number of tuples sent by all the sources
Atomic_Double average_latency_sum;          // sum of the average latency values measured in each of the sink's replicas
atomic<int> sink_zero_processed;            // number of sink's replicas that processed zero tuples

int main(int argc, char* argv[]) {
    /// parse arguments from command line
    int option = 0;
    int index = 0;

    string file_path;
    size_t source_par_deg = 0;
    size_t predictor_par_deg = 0;
    size_t sink_par_deg = 0;
    int rate = 0;
    sent_tuples = 0;
    sink_zero_processed = 0;

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

    /// application starting time
    unsigned long app_start_time = current_time_usecs();

    /// create the nodes
    Source_Functor source_functor(file_path, ",", rate, app_start_time);
    Source source = Source_Builder(source_functor)
            .withParallelism(source_par_deg)
            .withName(heavy_source_name)
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
    topology.chain_sink(sink);  // in order to exploit chaining, predictor and sink must have the same parallelism degree

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