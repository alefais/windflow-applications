/**
 * @file    cli_util.hpp
 * @author  Alessandra Fais
 * @date    30/04/2019
 *
 * @brief Util for command line options parsing
 *
 * This file contains functions and constants used for command line options parsing.
 */

#ifndef FRAUDDETECTION_CLI_UTIL_HPP
#define FRAUDDETECTION_CLI_UTIL_HPP

#include <string>

using namespace std;

typedef enum { NONE, REQUIRED } opt_arg;    // an option can require one argument or none

const struct option long_opts[] = {
        {"help", NONE, 0, 'h'},
        {"file", REQUIRED, 0, 'f'},         // file path
        {"nsource", REQUIRED, 0, 's'},      // pipe start (source)
        {"npredictor", REQUIRED, 0, 'p'},   // predictor
        {"nsink", REQUIRED, 0, 'e'},        // pipe end (sink)
        {"pardeg", REQUIRED, 0, 'n'},       // parallelism degree for all nodes
        {"rate", REQUIRED, 0, 'r'},         // stream generation rate
        {0, 0, 0, 0}
};

const string intro = "Run FraudDetection choosing one of the following ways:";
const string run_mode1 = " --file [file_path] --nsource [source_par_deg] --npredictor [predictor_par_deg] --nsink [sink_par_deg] --rate [stream_gen_rate]";
const string run_mode2 = " --file [file_path] --pardeg [par_deg_for_all_nodes] --rate [stream_gen_rate]";
const string run_help = " --help";

inline void print_help(char* arg) {
    cout << intro << endl
         << arg
         << run_mode1 << endl
         << arg
         << run_mode2 << endl
         << arg
         << run_help << endl;
}

#endif //FRAUDDETECTION_CLI_UTIL_HPP
