/**
 *  @file    predictor.hpp
 *  @author  Alessandra Fais
 *  @date    03/05/2019
 *
 *  @brief Node that implements the outliers detection
 */

#ifndef FRAUDDETECTION_PREDICTOR_HPP
#define FRAUDDETECTION_PREDICTOR_HPP

#include <ff/ff.hpp>
#include "../util/tuple.hpp"
#include "../util/result.hpp"
#include "../util/constants.hpp"

using namespace ff;
using namespace std;

class Predictor_Functor {
private:
    size_t processed;       // tuples counter

    // time variables
    unsigned long start_time;
    unsigned long current_time;

public:

    /**
     * Constructor.
     */
    Predictor_Functor(): processed(0) {
        // initialize time variables
        start_time = current_time_usecs();
        current_time = start_time;
    }

    void operator()(const tuple_t& t, Shipper<result_t>& shipper) {
        /*cout << "[Predictor] Received tuple: "
                 << t->entity_id << " - "
                 << t->record << ", "
                 << t->key << " - "
                 << t->id << " - "
                 << t->ts << endl;*/
        if (processed % 2 == 0) {
            result_t r;
            r.entity_id = t.entity_id;
            r.score = 0;
            r.states = "";
            r.key = t.key;
            r.id = t.id;
            r.ts = t.ts;
            shipper.push(r);
        }
        processed++;
    }

    ~Predictor_Functor() {}
};

#endif //FRAUDDETECTION_PREDICTOR_HPP
