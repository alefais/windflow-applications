/**
 * @file    cli_util.hpp
 * @author  Alessandra Fais
 * @date    14/05/2019
 *
 * @brief Util for parsing command line options and printing information on stdout
 *
 * This file contains functions and constants used for parsing command line options
 * and for showing information about the application on stdout.
 */

#ifndef SPIKEDETECTION_CLI_UTIL_HPP
#define SPIKEDETECTION_CLI_UTIL_HPP

#include <iostream>
#include <string>
#include <vector>
#include <getopt.h>

using namespace std;

typedef enum { NONE, REQUIRED } opt_arg;    // an option can require one argument or none

const struct option long_opts[] = {
        {"help", NONE, 0, 'h'},
        {"file", REQUIRED, 0, 'f'},         // input file path
        {"nsource", REQUIRED, 0, 's'},      // pipe start (source) parallelism degree
        {"naverage", REQUIRED, 0, 'a'},     // moving average calculator parallelism degree
        {"ndetector", REQUIRED, 0, 'd'},    // spike detector parallelism degree
        {"nsink", REQUIRED, 0, 'e'},        // pipe end (sink) parallelism degree
        {"pardeg", REQUIRED, 0, 'n'},       // parallelism degree for all nodes
        {"rate", REQUIRED, 0, 'r'},         // stream generation rate
        {0, 0, 0, 0}
};

// how to run the application
const string intro = "Run SpikeDetection choosing one of the following ways:";
const string run_mode1 = " --file [file_path] --nsource [source_par_deg] --naverage [avg_calc_par_deg] --ndetector [detector_par_deg] --nsink [sink_par_deg] --rate [stream_gen_rate]";
const string run_mode2 = " --file [file_path] --pardeg [par_deg_for_all_nodes] --rate [stream_gen_rate]";
const string run_help = " --help";

// information about application
const string app_descr = "Executing SpikeDetection with parameters:";
const string file_str = "* file path: ";
const string source_str = "* source parallelism degree: ";
const string avg_calc_str = "* average calculator parallelism degree: ";
const string detector_str = "* detector parallelism degree: ";
const string sink_str = "* sink parallelism degree: ";
const string rate_str = "* rate: ";

const string app_error = "Error executing SpikeDetection topology";
const string app_termination = "Terminated execution of SpikeDetection topology with cardinality ";

inline void print_help(char* arg) {
    cout << intro << endl
         << arg
         << run_mode1 << endl
         << arg
         << run_mode2 << endl
         << arg
         << run_help << endl;
}

inline void print_app_descr(string f, size_t source, size_t avg_calc, size_t detector, size_t sink, int rate) {
    cout << app_descr << endl
         << file_str << f << endl
         << source_str << source << endl
         << avg_calc_str << avg_calc << endl
         << detector_str << detector << endl
         << sink_str << sink << endl
         << rate_str << rate << endl;
}

#endif //SPIKEDETECTION_CLI_UTIL_HPP
