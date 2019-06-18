/**
 *  @file    counter.hpp
 *  @author  Alessandra Fais
 *  @date    18/06/2019
 *
 *  @brief Node that counts the occurrences of each word and the number of bytes processed
 */

#ifndef WORDCOUNT_COUNTER_HPP
#define WORDCOUNT_COUNTER_HPP

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
 *  @class Counter_Functor
 *
 *  @brief Define the logic of the Counter
 */
class Counter_Functor {
private:
    size_t processed;            // tuple counter (number of words)
    long bytes;                  // bytes counter

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
     Counter_Functor(const unsigned long _app_start_time):
            bytes(0L),
            app_start_time(_app_start_time),
            current_time(_app_start_time) {}

     void operator()(const result_t& in, result_t& out, RuntimeContext& rc) {
         if (processed == 0) {
             parallelism = rc.getParallelism();
             replica_id = rc.getReplicaIndex();
         }
         //print_result("[Counter] Received tuple: ", in);

         out.bytes += (in.key).length();    // size of the string word (bytes)
         out.key = in.key;
         out.id++;                          // number of occurrences of the string word
         out.ts = in.ts;

         bytes += out.bytes;
         processed++;
         current_time = current_time_usecs();
     }

     ~Counter_Functor() {
         /*if (processed != 0) {
             cout << "[Counter] replica " << replica_id + 1 << "/" << parallelism
                  << ", execution time: " << (current_time - app_start_time) / 1000000L
                  << " s, processed: " << processed << " words (" << (bytes / 1048576) << " MB)"
                  << ", bandwidth: " << processed / ((current_time - app_start_time) / 1000000L)
                  << " (words/s) " << (bytes / 1048576) / ((current_time - app_start_time) / 1000000L)
                  << " (MB/s)" << endl;
         }*/
     }
};

#endif //WORDCOUNT_COUNTER_HPP
