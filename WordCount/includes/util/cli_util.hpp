/**
 * @file    cli_util.hpp
 * @author  Alessandra Fais
 * @date    17/06/2019
 *
 * @brief Util for parsing command line options and printing information on stdout
 *
 * This file contains functions and constants used for parsing command line options
 * and for showing information about the application on stdout.
 */

#ifndef WORDCOUNT_CLI_UTIL_HPP
#define WORDCOUNT_CLI_UTIL_HPP

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <getopt.h>
#include "constants.hpp"
#include "tuple.hpp"
#include "result.hpp"

using namespace std;

typedef enum { NONE, REQUIRED } opt_arg;    // an option can require one argument or none

const struct option long_opts[] = {
        {"help", NONE, 0, 'h'},
        {"nsource", REQUIRED, 0, 's'},      // pipe start (source) parallelism degree
        {"nsplitter", REQUIRED, 0, 'p'},    // splitter parallelism degree
        {"ncounter", REQUIRED, 0, 'c'},     // word counter parallelism degree
        {"nsink", REQUIRED, 0, 'e'},        // pipe end (sink) parallelism degree
        {"pardeg", REQUIRED, 0, 'n'},       // parallelism degree for all nodes
        {"rate", REQUIRED, 0, 'r'},         // stream generation rate
        {0, 0, 0, 0}
};

// how to run the application
const string intro = "Run WordCount choosing one of the following ways:";
const string run_mode1 = " --nsource [source_par_deg] --nsplitter [splitter_par_deg] --ncounter [counter_par_deg] --nsink [sink_par_deg] --rate [stream_gen_rate]";
const string run_mode2 = " --pardeg [par_deg_for_all_nodes] --rate [stream_gen_rate]";
const string run_help = " --help";

// information about application
const string app_descr = "Executing WordCount with parameters:";
const string file_str = "* file path: ";
const string source_str = "* source parallelism degree: ";
const string splitter_str = "* splitter parallelism degree: ";
const string counter_str = "* counter parallelism degree: ";
const string sink_str = "* sink parallelism degree: ";
const string rate_str = "* rate: ";

const string app_error = "Error executing WordCount topology";
const string app_termination = "Terminated execution of WordCount topology with cardinality ";

inline void print_help(char* arg) {
    cout << intro << endl
         << arg
         << run_mode1 << endl
         << arg
         << run_mode2 << endl
         << arg
         << run_help << endl;
}

inline void print_app_descr(string& file, size_t source, size_t splitter, size_t counter, size_t sink, int rate) {
    cout << app_descr << endl
         << file_str << file << endl
         << source_str << source << endl
         << splitter_str << splitter << endl
         << counter_str << counter << endl
         << sink_str << sink << endl
         << rate_str << rate << endl;
}

inline void print_summary(long long total_bytes, double elapsed_time_seconds, double tot_average_latency) {
    long sent_MB = total_bytes / 1048576;
    long sent_GB = sent_MB / 1024;
    cout << "[SUMMARY] generated " << sent_MB << " (MB) " << sent_GB << " (GB)" << endl;
    cout << "[SUMMARY] elapsed time (seconds) " << elapsed_time_seconds << endl;
    cout << "[SUMMARY] bandwidth "
         << sent_MB / elapsed_time_seconds << " (MB/second) "
         << sent_GB / elapsed_time_seconds << " (GB/second) " << endl;
    cout << "[SUMMARY] average latency " << tot_average_latency << " (useconds), "
         << fixed << setprecision(5) << tot_average_latency / (1000.0) << " (ms) " <<  endl;
}

// information about parsed data and dataset (testing)
inline void print_dataset(const vector<tuple_t>& dataset) {
    cout << "[main] dataset size: " << dataset.size() << endl;
    for (auto t : dataset)
        cout << t.text_line << " - "
             << t.key << " - "
             << t.id << " - "
             << t.ts << endl;
}

// information about input tuple content (testing)
inline void print_tuple(const string& msg, const tuple_t& t) {
    cout << msg
         << t.text_line << ", "
         << t.key << " - "
         << t.id << " - "
         << t.ts << endl;
}

// information about result tuple content (testing)
inline void print_result(const string& msg, const result_t& r) {
    cout << msg
         << r.bytes << " - "
         << r.key << " - "
         << r.id << " - "
         << r.ts << endl;
}

#endif //WORDCOUNT_CLI_UTIL_HPP
