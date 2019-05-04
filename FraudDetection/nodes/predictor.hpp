/**
 *  @file    predictor.hpp
 *  @author  Alessandra Fais
 *  @date    04/05/2019
 *
 *  @brief Node that implements outliers detection
 */

#ifndef FRAUDDETECTION_PREDICTOR_HPP
#define FRAUDDETECTION_PREDICTOR_HPP

#include <ff/ff.hpp>
#include "../util/tuple.hpp"
#include "../util/result.hpp"
#include "../util/constants.hpp"

using namespace ff;
using namespace std;

/**
 *  @class Predictor_Functor
 *
 *  @brief Define the logic of the Predictor
 */
class Predictor_Functor {
private:
    size_t processed;       // tuples counter
    unordered_map<size_t, uint64_t> keys;

    // time variables
    unsigned long start_time;
    unsigned long current_time;

public:

    /**
     *  @brief Constructor
     */
    Predictor_Functor(): processed(0) {
        // initialize time variables
        start_time = current_time_usecs();
        current_time = start_time;
    }

    /**
     *  @brief Detect outliers
     *
     *  Given a transaction sequence of a customer, there is a probability associated with each path
     *  of state transition which indicates the chances of fraudolent activities. Only tuples for
     *  which an outlier has been identified are sent out.
     *  @param t input tuple
     *  @param shipper shipper object used for the delivery of results
     */
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

        if (keys.find(t.key) == keys.end())
            keys.insert(make_pair(t.key, t.id));
        else
            (keys.find(t.key))->second = t.id;
    }

    ~Predictor_Functor() {
        cout << "Received keys are: " << endl;
        for (auto k : keys) {
            cout << "key: " << k.first << " id: " << k.second << endl;
        }
    }
};

#endif //FRAUDDETECTION_PREDICTOR_HPP
