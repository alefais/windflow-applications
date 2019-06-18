/**
 *  @file    sink.hpp
 *  @author  Alessandra Fais
 *  @date    14/06/2019
 *
 *  @brief Sink node that receives and prints the results
 */

#ifndef TRAFFICMONITORING_SINK_HPP
#define TRAFFICMONITORING_SINK_HPP

#include <iomanip>
#include <ff/ff.hpp>
#include "../../includes/util/atomic_double.hpp"
#include "../util/cli_util.hpp"
#include "../util/result.hpp"

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
    size_t processed;                       // tuples counter
    vector<unsigned long> tuple_latencies;  // contains the latency of each received tuple

    /**
     * Evaluate the average latency value (average time needed by a tuple in order to
     * traverse the whole pipeline).
     *
     * @return average latency
     */
    double get_average_latency() {
        //if (rate == -1) return 0.0; // always evaluate latency with FF_BOUNDED_BUFFER set
        unsigned long acc = 0L;
        for (long tl : tuple_latencies) {
            acc += tl;
        }
        return ((double)acc / tuple_latencies.size());
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
            processed(0) {}

    /**
     * @brief Print results and evaluate average latency (when required)
     *
     * @param t input tuple
     */
    void operator()(optional<result_t>& r) {
        if (r) {
            //print_result("[Sink] received tuple: ", *r);

            // evaluate tuple latency (always evaluate latency with FF_BOUNDED_BUFFER set)
            //if (rate != -1) {

            unsigned long tuple_latency = current_time_usecs() - (app_start_time + (*r).ts);
            tuple_latencies.insert(tuple_latencies.end(), tuple_latency);

            //}
            processed++;        // tuples counter
        } else {     // EOS
            if (processed != 0) {
                /*cout << "[Sink] processed tuples: " << processed
                     << ", average latency: " << fixed << setprecision(5)
                     << get_average_latency()// / 1000 << " ms" << endl;
                     << " usecs" << endl;*/

                average_latency_sum.fetch_add(get_average_latency()); // add average latency value (useconds)
            } else {
                //cout << "[Sink] processed tuples: " << processed << endl;

                sink_zero_processed.fetch_add(1);
            }
        }
    }
};

#endif //TRAFFICMONITORING_SINK_HPP
