/**
 *  @file    counter.hpp
 *  @author  Alessandra Fais
 *  @date    07/09/2019
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
    size_t processed;                                   // tuple counter (number of words)
    long bytes;                                         // bytes counter
    unordered_map<string, uint64_t> word_occurrences;   // keeps the number of occurrences for each word

    // time variables
    unsigned long app_start_time;
    unsigned long current_time;
    //unsigned long temp_time;

    // runtime information
    size_t parallelism;
    size_t replica_id;

public:

    /**
     *  @brief Constructor
     */
     Counter_Functor(const unsigned long _app_start_time):
            bytes(0L),
            processed(0),
            app_start_time(_app_start_time), //temp_time(0)
            current_time(_app_start_time) {}

     void operator()(const result_t& in, result_t& out, RuntimeContext& rc) {
         //temp_time = current_time_usecs();

         if (processed == 0) {
             parallelism = rc.getParallelism();
             replica_id = rc.getReplicaIndex();
         }
         //print_result("[Counter] Received tuple: ", in);

         if (word_occurrences.find(in.key) == word_occurrences.end())
             word_occurrences.insert(make_pair(in.key, 1));
         else
             word_occurrences.at(in.key)++;

         out.key = in.key;
         out.id = word_occurrences.at(in.key);          // number of occurrences of the string word
         out.ts = in.ts;

         bytes += (in.key).length();                    // size of the string word (bytes)
         processed++;
         current_time = current_time_usecs();
     }

     ~Counter_Functor() {
         if (processed != 0) {
             //cout << "[Counter] number of different words in map: " << word_occurrences.size() << endl;
             //cout << "[Counter] service time: " << (current_time - temp_time)<< " us" << endl;
             /*for (auto el : word_occurrences) {
                 cout << "<" << el.first << ", " << el.second << "> " << endl;
             }*/
             /*cout << "[Counter] replica " << replica_id + 1 << "/" << parallelism
                  << ", execution time: " << (current_time - app_start_time) / 1000000L
                  << " s, processed: " << processed << " words (" << ((double)bytes / 1048576) << " MB)"
                  << ", bandwidth: " << processed / ((current_time - app_start_time) / 1000000L)
                  << " (words/s) " << ((double)bytes / 1048576) / ((double)(current_time - app_start_time) / 1000000L)
                  << " (MB/s)" << endl;*/
         }
     }
};

#endif //WORDCOUNT_COUNTER_HPP
