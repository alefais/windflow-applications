/**
 *  @file    sink.hpp
 *  @author  Alessandra Fais
 *  @date    04/09/2019
 *
 *  @brief Sink node that receives and prints the number of occurrences of each word in the input text
 */

#ifndef WORDCOUNT_SINK_HPP
#define WORDCOUNT_SINK_HPP

#include <algorithm>
#include <iomanip>
#include <ff/ff.hpp>
#include "statistics.h"
#include "../../includes/util/atomic_double.hpp"
#include "../util/cli_util.hpp"
#include "../util/result.hpp"

using namespace std;

extern Atomic_Double total_MB;
extern atomic<long> total_words;
extern Atomic_Double average_latency_sum;
extern atomic<int> sink_zero_processed;

/**
 *  @class Sink_Functor
 *
 *  @brief Defines the logic of the Sink
 */
class Sink_Functor {
private:
    int rate;                               // stream generation rate
    unsigned long app_start_time;
    unsigned long current_time;
    size_t words;                           // tuples (words) counter
    double bytes_sum;                       // bytes counter
    vector<double> tuple_latencies;         // contains the latency (ms) of each received tuple

    /**
     * Evaluate latency statistics:
     * - mean value (average time needed by a tuple in order to traverse the whole pipeline),
     * - 5th, 25th, 50th, 75th, 95th percentiles,
     * - minimum value,
     * - maximum value.
     */
    double compute_latency_statistics() {
        // vector containing all latency values
        alglib::real_1d_array tuple_lat_array;
        tuple_lat_array.setcontent(tuple_latencies.size(), &(tuple_latencies.at(0)));

        // percentiles
        double perc_5 = 0.05;
        double perc_25 = 0.25;
        double perc_50 = 0.5;
        double perc_75 = 0.75;
        double perc_95 = 0.95;

        // statistics
        double mean;
        double min;
        double perc_5_val;
        double perc_25_val;
        double perc_50_val;
        double perc_75_val;
        double perc_95_val;
        double max;

        // statistics computation
        mean = alglib::samplemean(tuple_lat_array);
        alglib::samplepercentile(tuple_lat_array, perc_5, perc_5_val);
        alglib::samplepercentile(tuple_lat_array, perc_25, perc_25_val);
        alglib::samplepercentile(tuple_lat_array, perc_50, perc_50_val);
        alglib::samplepercentile(tuple_lat_array, perc_75, perc_75_val);
        alglib::samplepercentile(tuple_lat_array, perc_95, perc_95_val);
        min = *min_element(tuple_latencies.begin(), tuple_latencies.end());
        max = *max_element(tuple_latencies.begin(), tuple_latencies.end());

        // latency summary
        cout << "[Sink] latency (ms): "
             << mean << " (mean) "
             << min << " (min) "
             << perc_5_val << " (5th) "
             << perc_25_val << " (25th) "
             << perc_50_val << " (50th) "
             << perc_75_val << " (75th) "
             << perc_95_val << " (95th) "
             << max << " (max)." << endl;

        return mean;
    }

public:

    /**
     *  @brief Constructor
     *
     *  @param _rate stream generation rate
     *  @param _app_start_time application starting time
     */
    Sink_Functor(const int _rate,
                 const unsigned long _app_start_time):
            rate(_rate),
            app_start_time(_app_start_time),
            current_time(_app_start_time),
            words(0),
            bytes_sum(0) {}

    /**
     * @brief Print results and evaluate latency statistics
     *
     * @param r input tuple
     */
    void operator()(optional<result_t>& r) {
        if (r) {
            //print_result("[Sink] received tuple: ", *r);

            words++;                            // tuples counter
            bytes_sum += (*r).key.length();     // bytes counter

            // always evaluate latency when compiling with FF_BOUNDED_BUFFER MACRO set
            unsigned long tuple_latency = current_time_usecs() - (app_start_time + (*r).ts);    // latency (usecs)

            // consider a sample of 1M tuples to compute latency statistics (sequential computation)
            if (tuple_latencies.size() < 1000000)
                tuple_latencies.insert(tuple_latencies.end(), (double)tuple_latency / 1000L);       // latency (ms)

            current_time = current_time_usecs();
        } else {     // EOS
            if (words != 0) {
                unsigned long t_elapsed = (current_time - app_start_time) / 1000000L;

                cout << "[Sink] processed: "
                     << words << " (words) "
                     << (bytes_sum / 1048576) << " (MB), "
                     << "bandwidth: "
                     << words / t_elapsed << " (words/s) "
                     << (bytes_sum / 1048576) / t_elapsed << " (MB/s)." << endl;

                unsigned long lat_stats_begin = current_time_usecs();
                double average_latency = compute_latency_statistics();
                unsigned long lat_stats_time = current_time_usecs() - lat_stats_begin;

                cout << "[Sink] latency samples "
                     << tuple_latencies.size()
                     << " and time to compute stats "
                     << lat_stats_time / 1000L
                     << " ms." << endl;

                total_MB.fetch_add(bytes_sum / 1048576);
                total_words.fetch_add(words);
                average_latency_sum.fetch_add(average_latency);
            } else {
                //cout << "[Sink] processed: " << words << endl;
                sink_zero_processed.fetch_add(1);
            }
        }
    }
};

#endif //WORDCOUNT_SINK_HPP
