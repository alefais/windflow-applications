/**
 * @file    cli_util.hpp
 * @author  Alessandra Fais
 * @date    04/05/2019
 *
 * @brief Util for parsing command line options and printing information on stdout
 *
 * This file contains functions and constants used for parsing command line options
 * and for showing information about the application state on stdout.
 */

#ifndef FRAUDDETECTION_CLI_UTIL_HPP
#define FRAUDDETECTION_CLI_UTIL_HPP

#include <string>
#include <getopt.h>

using namespace std;

typedef enum { NONE, REQUIRED } opt_arg;    // an option can require one argument or none

const struct option long_opts[] = {
        {"help", NONE, 0, 'h'},
        {"file", REQUIRED, 0, 'f'},         // input file path
        {"nsource", REQUIRED, 0, 's'},      // pipe start (source) parallelism degree
        {"npredictor", REQUIRED, 0, 'p'},   // predictor parallelism degree
        {"nsink", REQUIRED, 0, 'e'},        // pipe end (sink) parallelism degree
        {"pardeg", REQUIRED, 0, 'n'},       // parallelism degree for all nodes
        {"rate", REQUIRED, 0, 'r'},         // stream generation rate
        {0, 0, 0, 0}
};

// how to run the application
const string intro = "Run FraudDetection choosing one of the following ways:";
const string run_mode1 = " --file [file_path] --nsource [source_par_deg] --npredictor [predictor_par_deg] --nsink [sink_par_deg] --rate [stream_gen_rate]";
const string run_mode2 = " --file [file_path] --pardeg [par_deg_for_all_nodes] --rate [stream_gen_rate]";
const string run_help = " --help";

// information about application
const string app_descr = "Executing FraudDetection with parameters:";
const string file_str = "* file path: ";
const string source_str = "* source parallelism degree: ";
const string predictor_str = "* predictor parallelism degree: ";
const string sink_str = "* sink parallelism degree: ";
const string rate_str = "* rate: ";

const string app_error = "Error executing FraudDetection topology";
const string app_termination = "Terminated execution of FraudDetection topology with cardinality ";

inline void print_help(char* arg) {
    cout << intro << endl
         << arg
         << run_mode1 << endl
         << arg
         << run_mode2 << endl
         << arg
         << run_help << endl;
}

inline void print_app_descr(string f, size_t source, size_t pred, size_t sink, int rate) {
    cout << app_descr << endl
         << file_str << f << endl
         << source_str << source << endl
         << predictor_str << pred << endl
         << sink_str << sink << endl
         << rate_str << rate << endl;
}

inline void print_model_parameters(const string& _model_file, size_t records_win_size, size_t state_position, int alg, double threshold) {
    cout << "Model parameters are: "
         << " file " << _model_file << endl
         << " records_win_size " << records_win_size << endl
         << " state_position (in the record string) " << state_position << endl
         << " algorithm " << alg << endl
         << " threshold " << threshold << endl;
}

inline void print_window(const vector<string>& states_sequence) {
    cout << "Compute local metric for states in window [ ";
    for (auto s : states_sequence) cout << s << " ";
    cout << "]" << endl;
}

inline void print_prob_indexes(const string& prev_state, const string& cur_state, size_t prev_state_idx, size_t cur_state_idx) {
    cout << "State probability indexes are: "
         << "[prev state] " << prev_state << " at " << prev_state_idx
         << ", [cur state] " << cur_state << " at " << cur_state_idx << endl;
}

inline void print_fraudolent_sequence(const vector<string>& states_sequence, double score, double threshold) {
    cout << "Fraudolent sequence: ";
    for (string s : states_sequence) {
        cout << s << " ";
    }
    cout << endl << "Score: " << score << " Threshold: " << threshold << endl;
}

#endif //FRAUDDETECTION_CLI_UTIL_HPP
