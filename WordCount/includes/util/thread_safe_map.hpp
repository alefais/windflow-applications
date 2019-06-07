/**
 *  @file    thread_safe_map.hpp
 *  @author  Alessandra Fais
 *  @date    07/06/2019
 *
 *  @brief Class implementing thread safe access to unordered map (shared state accessed by splitter replicas)
 *
 *  The class is used to count the number of occurrences of each word in the whole input file.
 *  All the replicas of the splitter node increment the counter value each time the corresponding
 *  word is encountered and processed. The occurrences value read from the map is used to fill the
 *  id field of the emitted tuples of type result_t. In this way the sink will always receive correct
 *  values of the id fields for all the keys.
 */
#ifndef WORDCOUNT_THREAD_SAFE_MAP_HPP
#define WORDCOUNT_THREAD_SAFE_MAP_HPP

#include <ff/ff.hpp>
#include <windflow.hpp>

using namespace std;

class Thread_Safe_Map {
private:
    unordered_map<string, long> word_occ;    // number of occurrences of the words in the input text
    mutex m;                                 // mutex

public:
    Thread_Safe_Map() {}

    long fetch_incr(const string& word) {
        unique_lock<mutex> lock(m);
        if (word_occ.find(word) == word_occ.end())  // insert the word in the map (if it is not present)
            word_occ.insert(make_pair(word, 0));
        word_occ.at(word)++;

        return word_occ.at(word);                   // return updated counter value
    }

    ~Thread_Safe_Map() {}
};

#endif //WORDCOUNT_THREAD_SAFE_MAP_HPP
