/**
 *  @file    sink.hpp
 *  @author  Alessandra Fais
 *  @date    11/05/2019
 *
 *  @brief Sink node that receives and prints the results
 */

#ifndef FRAUDDETECTION_SINK_HPP
#define FRAUDDETECTION_SINK_HPP

#include <iomanip>
#include <ff/ff.hpp>
#include "../util/tuple.hpp"

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
        if (rate == -1) return 0.0;
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
    void operator()(optional<result_t>& t) {
        if (t) {
            cout << "[Sink] Received tuple: "
                 << t->entity_id << " - "
                 << t->score << " - [ "
                 << t->states << "], "
                 << t->key << " - "
                 << t->id << " - "
                 << t->ts << endl;

            // evaluate tuple latency
            if (rate != -1) {
                unsigned long tuple_latency = current_time_usecs() - (app_start_time + (*t).ts);
                tuple_latencies.insert(tuple_latencies.end(), tuple_latency);
            }
            processed++;        // tuples counter
        } else {     // EOS
            if (processed != 0) {
                cout << "[Sink] processed tuples: " << processed
                     << ", average latency: " << fixed << setprecision(5)
                     << get_average_latency()// / 1000 << " ms" << endl;
                     << " usecs" << endl;
            } else cout << "[Sink] processed tuples: " << processed << endl;
        }
    }
};

#endif //FRAUDDETECTION_SINK_HPP
