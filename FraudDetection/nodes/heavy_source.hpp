/**
 *  @file    heavy_source.hpp
 *  @author  Alessandra Fais
 *  @date    07/05/2019
 *
 *  @brief Source node that generates the input stream
 */

#ifndef FRAUDDETECTION_HEAVY_SOURCE_HPP
#define FRAUDDETECTION_HEAVY_SOURCE_HPP

#include <fstream>
#include <vector>
#include <ff/ff.hpp>
#include "../util/tuple.hpp"
#include "../util/constants.hpp"

using namespace ff;
using namespace std;
using count_key_t = pair<size_t, uint64_t>;
using key_map_t = unordered_map<string, count_key_t>;

/**
 *  @class Source_Functor
 *
 *  @brief Define the logic of the Source
 */
class Source_Functor {
private:
    string file_path;                           // path of the input file
    string split_regex;                         // regular expression
    int rate;                                   // stream generation rate
    key_map_t entity_key_map;                   // contains a mapping between string keys and integer keys for each entity_id
    size_t entity_unique_key;                   // unique integer key
    vector<pair<string, string>> parsed_file;   // contains strings extracted from the input file
    size_t next_tuple_idx;                      // index of the next tuple to be sent
    int generations;                            // counts the times the file is generated
    long generated_tuples;                      // tuples counter

    // time variables
    unsigned long app_start_time;
    unsigned long start_time;
    unsigned long current_time;
    unsigned long interval;

    /**
     *  @brief Map keys and parse the input file
     *
     *  This method assigns to each string key entity_id a unique integer key (required by the current
     *  implementation of WindFlow). Moreover, the file is parsed and saved in memory.
     */
    void map_and_parse() {
        ifstream file(file_path);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                string entity_id = line.substr(0, line.find(split_regex));
                string record = line.substr(line.find(split_regex) + 1, line.size());

                // map keys
                if (entity_key_map.find(entity_id) == entity_key_map.end()) { // the key is not present in the hash map
                    entity_key_map.insert(make_pair(entity_id, make_pair(entity_unique_key, 0)));
                    entity_unique_key++;
                }

                // save parsed file
                parsed_file.insert(parsed_file.end(), make_pair(entity_id, record));
            }
            file.close();
        }
    }

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
     *  @param _file_path path of the input dataset file
     *  @param _split_regex regular expression used to parse the file (it depends on the input file format
     *                      e.g. for a file input.csv the regular expression to be used is ",")
     *  @param _rate stream generation rate
     *  @param _app_start_time application starting time
     */
    Source_Functor(const string& _file_path,
                   const string& _split_regex,
                   const int _rate,
                   const unsigned long _app_start_time):
                   file_path(_file_path),
                   split_regex(_split_regex),
                   rate(_rate),
                   app_start_time(_app_start_time),
                   current_time(_app_start_time),
                   entity_unique_key(0),
                   next_tuple_idx(0),
                   generations(0),
                   generated_tuples(0)
    {
        // initialize time variables
        interval = 1000000L; // 1 second (microseconds)
        start_time = current_time_usecs();

        map_and_parse();
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

        // send tuple
        auto tuple_content = parsed_file.at(next_tuple_idx);
        t.entity_id = tuple_content.first;
        t.record = tuple_content.second;
        t.key = (entity_key_map.find(t.entity_id)->second).first;
        t.id = ((entity_key_map.find(t.entity_id))->second).second++;
        t.ts = current_time - app_start_time;

        if (rate != -1) // stream generation rate is fixed
            active_delay(interval / rate);

        next_tuple_idx = (next_tuple_idx + 1) % parsed_file.size();   // index of the next tuple to be sent (if any)

        // EOS reached
        if (current_time - app_start_time >= app_run_time && next_tuple_idx == 0) {
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

#endif //FRAUDDETECTION_HEAVY_SOURCE_HPP
