/**
 *  @file    splitter.hpp
 *  @author  Alessandra Fais
 *  @date    16/07/2019
 *
 *  @brief Node that splits each received text line into single words
 */

#ifndef WORDCOUNT_SPLITTER_HPP
#define WORDCOUNT_SPLITTER_HPP

#include <ff/ff.hpp>
#include <string>
#include <vector>
#include <regex>
#include "../util/tuple.hpp"
#include "../util/result.hpp"
#include "../util/constants.hpp"
#include "../util/cli_util.hpp"

using namespace std;

/**
 *  @class Splitter_Functor
 *
 *  @brief Define the logic of the Splitter
 */
class Splitter_Functor {
private:
    size_t processed;       // tuples counter (number of text lines processed)
    size_t words;           // words counter (number of words produced as output of the line splitting process)

    // time variables
    unsigned long app_start_time;
    unsigned long current_time;

    // runtime information
    size_t parallelism;
    size_t replica_id;

public:

    /**
     *  @brief Constructor
     */
    Splitter_Functor(const unsigned long _app_start_time):
            processed(0),
            words(0),
            app_start_time(_app_start_time),
            current_time(_app_start_time) {}

    void operator()(const tuple_t& t, Shipper<result_t>& shipper, RuntimeContext& rc) {
        if (processed == 0) {
            parallelism = rc.getParallelism();
            replica_id = rc.getReplicaIndex();
        }
        //print_tuple("[Splitter] Received tuple: ", t);

        regex rgx("\\s+"); // regex quantifier (matches one or many whitespaces)
        sregex_token_iterator iter(t.text_line.begin(), t.text_line.end(), rgx, -1);
        sregex_token_iterator end;

        result_t r;
        while (iter != end) {
            // save the number of occurences of each word found after line splitting process
            // and fill the output tuple fields
            r.key = *iter;                          // word key
            r.ts = t.ts;                            // timestamp
            shipper.push(r);
            words++;
            iter++;

            //print_result("[Splitter] Sent tuple: ", r);
        }
        processed++;
        current_time = current_time_usecs();
    }

     ~Splitter_Functor() {
         if (processed != 0) {
             /*cout << "[Splitter] replica " << replica_id + 1 << "/" << parallelism
                  << ", execution time: " << (current_time - app_start_time) / 1000000L
                  << " s, processed: " << processed << " lines (" << words << " words)"
                  << ", bandwidth: " << words / ((current_time - app_start_time) / 1000000L)
                  << " words/s" << endl;*/
         }
     }
};

#endif //WORDCOUNT_SPLITTER_HPP
