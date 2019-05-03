/**
 *  @file    source.hpp
 *  @author  Alessandra Fais
 *  @date    03/05/2019
 *
 *  @brief Sink node that receives and prints the results
 */

#ifndef FRAUDDETECTION_SINK_HPP
#define FRAUDDETECTION_SINK_HPP

#include <iomanip>
#include <ff/ff.hpp>
#include "../util/tuple.hpp"

class Sink_Functor {
private:
    int rate;                               // stream generation rate
    size_t processed;                       // tuples counter
    vector<unsigned long> tuple_latencies;  // contains the latency of each received tuple

    /**
     * Evaluate the average latency value (average time needed by a tuple in order to
     * traverse the whole pipeline).
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
     *  Constructor.
     *  @param _rate stream generation rate
     */
    Sink_Functor(const int _rate): rate(_rate), processed(0) {}

    void operator()(optional<tuple_t>& t) {
        if (t) {
            /*cout << "Received tuple: "
                 << t->entity_id << " - "
                 << t->record << ", "
                 << t->key << " - "
                 << t->id << " - "
                 << t->ts << endl;*/

            // evaluate tuple latency
            if (rate != -1) {
                unsigned long tuple_latency = current_time_usecs() - (*t).ts;
                tuple_latencies.insert(tuple_latencies.end(), tuple_latency);
            }
            processed++;        // tuples counter
        } else {     // EOS
            cout << "[Sink] processed tuples: " << processed
                 << ", average latency: " << fixed << setprecision(3) << get_average_latency() / 1000 << " ms" << endl;
        }
    }
};

#endif //FRAUDDETECTION_SINK_HPP
