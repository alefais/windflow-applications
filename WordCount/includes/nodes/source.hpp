/**
 *  @file    source.hpp
 *  @author  Alessandra Fais
 *  @date    06/06/2019
 *
 *  @brief Source node that generates the input stream
 *
 *  The source node generates the stream by reading the tuples from memory.
 */

#ifndef WORDCOUNT_SOURCE_HPP
#define WORDCOUNT_SOURCE_HPP

#include <fstream>
#include <vector>
#include <ff/ff.hpp>
#include "../util/tuple.hpp"
#include "../util/constants.hpp"
#include "../util/cli_util.hpp"

using namespace std;

/**
 *  @class Source_Functor
 *
 *  @brief Define the logic of the Source
 */
class Source_Functor {
private:
    vector<tuple_t> dataset;        // contains all the tuples
    int rate;                       // stream generation rate
    size_t next_tuple_idx;          // index of the next tuple to be sent
    int generations;                // counts the times the file is generated
    long generated_tuples;          // tuples counter

    // time variables
    unsigned long app_start_time;   // application start time
    unsigned long start_time;       // source start time
    unsigned long current_time;
    unsigned long interval;

    /**
     *  @brief Add some active delay (busy-waiting function)
     *
     *  @param waste_time wait time in microseconds
     */
    void active_delay(unsigned long waste_time) {
        auto start_time = current_time_usecs();
        bool end = false;
        while (!end) {
            auto end_time = current_time_usecs();
            end = (end_time - start_time) >= waste_time;
        }
    }

public:
    /**
     *  @brief Constructor.
     *
     *  @param _dataset all the tuples that will compose the stream
     *  @param _rate stream generation rate
     *  @param _app_start_time application starting time
     */
    Source_Functor(const vector<tuple_t>& _dataset,
                   const int _rate,
                   const unsigned long _app_start_time):
            rate(_rate),
            app_start_time(_app_start_time),
            current_time(_app_start_time),
            next_tuple_idx(0),
            generations(0),
            generated_tuples(0)
    {
        dataset = _dataset;

        // initialize time variables
        interval = 1000000L; // 1 second (microseconds)
        start_time = current_time_usecs();
    }

    /**
     *  @brief Send tuples in a item-by-item fashion
     *
     *  @param t reference to the tuple structure
     *  @return true if the stream is not ended, false if the EOS has been reached
     */
    bool operator()(tuple_t& t) {
        if (generated_tuples > 0) current_time = current_time_usecs();
        if (next_tuple_idx == 0) generations++;         // file generations counter
        generated_tuples++;                             // tuples counter

        // put a timestamp and send the tuple
        tuple_t tuple = dataset.at(next_tuple_idx);
        t.text_line = tuple.text_line;
        t.key = tuple.key;
        t.id = tuple.id;
        t.ts = current_time - app_start_time;

        //print_tuple("[Source] tuple content: ", t);

        if (rate != -1) // stream generation rate is fixed
            active_delay(interval / rate);

        next_tuple_idx = (next_tuple_idx + 1) % dataset.size();   // index of the next tuple to be sent (if any)

        // EOS reached
        if (current_time - start_time >= app_run_time && next_tuple_idx == 0) {
            cout << "[Source] execution time: " << (current_time - start_time) / 1000000L
                 << " s, generations: " << generations
                 << ", generated: " << generated_tuples
                 << ", bandwidth: " << generated_tuples / ((current_time - start_time) / 1000000L)
                 << " tuples/s" << endl;
            return false;
        }
        return true;         // stream not ended yet
    }

    ~Source_Functor() {}
};

#endif //WORDCOUNT_SOURCE_HPP
