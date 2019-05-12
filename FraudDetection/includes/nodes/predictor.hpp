/**
 *  @file    predictor.hpp
 *  @author  Alessandra Fais
 *  @date    11/05/2019
 *
 *  @brief Node that implements outliers detection
 */

#ifndef FRAUDDETECTION_PREDICTOR_HPP
#define FRAUDDETECTION_PREDICTOR_HPP

#include <ff/ff.hpp>
#include "../util/tuple.hpp"
#include "../util/result.hpp"
#include "../util/constants.hpp"
#include "../markov_model_prediction/model_based_predictor.hpp"

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
    size_t outliers;
    Markov_Model_Predictor predictor;
    unordered_map<size_t, uint64_t> keys;

    // time variables
    unsigned long start_time;
    unsigned long current_time;

    // runtime information
    size_t parallelism;
    size_t replica_id;

public:

    /**
     *  @brief Constructor
     */
    Predictor_Functor(): processed(0), outliers(0) {
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
    void operator()(const tuple_t& t, Shipper<result_t>& shipper, RuntimeContext rc) {
        if (processed == 0) {
            parallelism = rc.getParallelism();
            replica_id = rc.getReplicaIndex();
        }
        /*cout << "[Predictor] Received tuple: "
                 << t.entity_id << " - "
                 << t.record << ", "
                 << t.key << " - "
                 << t.id << " - "
                 << t.ts << endl;*/

        Prediction prediction_object = predictor.execute(t.entity_id, t.record, ",");
        if (prediction_object.is_outlier()) {
            result_t r;
            r.entity_id = t.entity_id;
            r.score = prediction_object.get_score();
            r.states = prediction_object.get_states();
            r.key = t.key;
            r.id = t.id;
            r.ts = t.ts;
            shipper.push(r);
            outliers++;
        }
        processed++;

        // save the received keys (test keyed distribution)
        if (keys.find(t.key) == keys.end())
            keys.insert(make_pair(t.key, t.id));
        else
            (keys.find(t.key))->second = t.id;

        current_time = current_time_usecs();
    }

    ~Predictor_Functor() {
        if (processed != 0) {
            cout << "[Predictor] replica " << replica_id + 1 << "/" << parallelism
                 << ", execution time: " << (current_time - start_time) / 1000000L
                 << " s, processed: " << processed
                 << ", outliers: " << outliers
                 << ", bandwidth: " << processed / ((current_time - start_time) / 1000000L)
                 << ", #keys: " << keys.size()
                 << endl;
                 /*<< ", keys: "
                 << endl;
            for (auto k : keys) {
                cout << "key: " << k.first << " id: " << k.second << endl;
            }*/
        }
    }
};

#endif //FRAUDDETECTION_PREDICTOR_HPP
