/**
 *  @file    constants.hpp
 *  @author  Alessandra Fais
 *  @date    14/05/2019
 *
 *  @brief Definition of useful constants
 */

#ifndef SPIKEDETECTION_CONSTANTS_HPP
#define SPIKEDETECTION_CONSTANTS_HPP

#include <string>

using namespace std;

/// application run time (source generates the stream for app_run_time seconds, then sends out EOS)
unsigned long app_run_time = 10 * 1000000L; // 10 seconds

/// components and topology name
const string topology_name = "SpikeDetection";
const string source_name = "source";
const string avg_calc_name = "average_calculator";
const string detector_name = "detector";
const string sink_name = "sink";

/// model parameters
typedef enum { TEMPERATURE, HUMIDITY, LIGHT, VOLTAGE } monitored_field;

size_t moving_avg_win_size = 1000;
monitored_field _field = TEMPERATURE;
double _threshold = 0.025;

const string _input_file = "../data/sensors.dat";

#endif //SPIKEDETECTION_CONSTANTS_HPP
