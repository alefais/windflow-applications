/**
 *  @file    main_heavy_source.cpp
 *  @author  Alessandra Fais
 *  @date    07/05/2019
 *
 *  @brief main of the FraudDetection application
 */

#include <iostream>
#include <string>
#include <ff/ff.hpp>
#include <windflow.hpp>

#include "util/cli_util.hpp"
#include "nodes/heavy_source.hpp"
#include "nodes/predictor.hpp"
#include "nodes/sink.hpp"

using namespace std;

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
            .keyBy()
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
        cout << app_termination << topology.cardinality() << endl;

    return 0;
}