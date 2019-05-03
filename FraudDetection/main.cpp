/**
 *  @file    main.cpp
 *  @author  Alessandra Fais
 *  @date    03/05/2019
 *
 *  @brief main of the FraudDetection application
 */

#include <iostream>
#include <string>
#include <getopt.h>
#include <ff/ff.hpp>
#include <windflow.hpp>

#include "util/cli_util.hpp"
#include "nodes/source.hpp"
#include "nodes/sink.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    /// parse arguments from command line
    int option = 0;
    int index = 0;

    string file_path;
    size_t source_par_deg;
    size_t predictor_par_deg;
    size_t sink_par_deg;
    size_t all_par_deg;
    int rate;

    /*
    OptionsParser op(argc, argv);
    const string& file_name = op.getOption("-file");
    if (!file_name.empty()){
        cout << "Parsed file name " << file_name << endl;
    }*/

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
        // check values
        cout << "You pass file " << file_path << endl;
        cout << "You pass nsource " << source_par_deg << endl;
        cout << "You pass npredictor " << predictor_par_deg << endl;
        cout << "You pass nsink " << sink_par_deg << endl;
        cout << "You pass rate " << rate << endl;
    } else if (argc == 7) {
        while ((option = getopt_long(argc, argv, "f:n:r:", long_opts, &index)) != -1) {
            switch (option) {
                case 'f': file_path = optarg;
                    break;
                case 'n': all_par_deg = atoi(optarg);
                    break;
                case 'r': rate = atoi(optarg);
                    break;
                default:
                    print_help(argv[0]);
                    exit(EXIT_SUCCESS);
            }
        }
        // check values
        cout << "You pass file " << file_path << endl;
        cout << "You pass pardeg " << all_par_deg << endl;
        cout << "You pass rate " << rate << endl;
    } else if (argc == 2) {
        while ((getopt_long(argc, argv, "h", long_opts, &index)) != -1) {
            print_help(argv[0]);
            exit(EXIT_SUCCESS);
        }
    } else {
        print_help(argv[0]);
        exit(EXIT_SUCCESS);
    }

    /// create the nodes
    Source_Functor source_functor(file_path, ",", rate);
    Source source = Source_Builder(source_functor)
            .withParallelism(all_par_deg) //(source_par_deg)
            .withName("source")
            .build();

    Sink_Functor sink_functor(rate);
    Sink sink = Sink_Builder(sink_functor)
            .withParallelism(all_par_deg) //(sink_par_deg)
            .withName("sink")
            .build();

    /// create the multi pipe
    MultiPipe topology("FraudDetection");
    topology.add_source(source);
    topology.add_sink(sink);
    if (topology.run_and_wait_end() < 0)
        cerr << "Error executing FraudDetection topology" << endl;
    else
        cout << "Terminated execution of FraudDetection topology with cardinality " << topology.cardinality() << endl;

    return 0;
}