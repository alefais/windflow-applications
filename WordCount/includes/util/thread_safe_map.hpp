/**
 *  @file    thread_safe_map.hpp
 *  @author  Alessandra Fais
 *  @date    07/06/2019
 *
 *  @brief Class implementing thread safe unordered map
 *
 *  The class is used to count the number of occurrences of each word in the whole input file.
 *  All the replicas of the splitter node increment the counter value each time the
 *  corresponding word is encountered and processed. The data structure is used to implement a
 *  shared state: all the splitter replicas concurrently access the data structure to read and write.
 *  hat guarantees that each splitter replica put the right value in the id field
 *  of the emitted tuples of type result_t.
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
        long old_value = word_occ.at(word);
        word_occ.at(word)++;

        return old_value;
    }

    long get(const string& word) {
        unique_lock<mutex> lock(m);
        return word_occ.at(word);
    }

    ~Thread_Safe_Map() {}
};

#endif //WORDCOUNT_THREAD_SAFE_MAP_HPP
