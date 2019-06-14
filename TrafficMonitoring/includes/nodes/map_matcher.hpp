/**
 *  @file    map_matcher.hpp
 *  @author  Alessandra Fais
 *  @date    14/06/2019
 *
 *  @brief Node that implements map matching
 *
 *  Map matching is the problem of how to match recorded geographic coordinates to a logical
 *  model of the real world, typically using some form of Geographic Information System.
 */

#ifndef TRAFFICMONITORING_MAP_MATCHER_HPP
#define TRAFFICMONITORING_MAP_MATCHER_HPP

#include <ff/ff.hpp>

#include "ogr_geometry.h"
#include "../util/tuple.hpp"
#include "../util/result.hpp"
#include "../util/constants.hpp"
#include "../util/cli_util.hpp"
#include "../geographic_model/road_grid_list.hpp"

using namespace ff;
using namespace std;

/**
 *  @class Map_Matcher_Functor
 *
 *  @brief Define the logic of the Map Matcher
 */
class Map_Matcher_Functor {
private:
    size_t processed;                            // counter of processed tuples
    size_t valid_points;                         // counter of tuples containing GPS coordinates (points) laying inside the city bounding box
    size_t emitted;                              // counter of tuples containing points that correspond to a valid road_id
    Road_Grid_List road_grid_list;               // object containing all the geometric features of the shapefile and used to do map matching
    unordered_map<size_t, uint64_t> key_occ;     // contains the number of occurrences of each key road_id

    // geographic model parameters: shapefile and city bounding box
    const string shapefile_path = (_monitored_city == DUBLIN) ? _dublin_shapefile : _beijing_shapefile;
    const double max_lon = (_monitored_city == DUBLIN) ? dublin_lon_max : beijing_lon_max;
    const double min_lon = (_monitored_city == DUBLIN) ? dublin_lon_min : beijing_lon_min;
    const double max_lat = (_monitored_city == DUBLIN) ? dublin_lat_max : beijing_lat_max;
    const double min_lat = (_monitored_city == DUBLIN) ? dublin_lat_min : beijing_lat_min;

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
    Map_Matcher_Functor(): road_grid_list(shapefile_path), processed(0), valid_points(0), emitted(0) {
        // initialize time variables
        start_time = current_time_usecs();
        current_time = start_time;
    }

    /**
     *  @brief Compute a road ID matching the GPS coordinates of a vehicle
     *
     *  Evaluate for each input tuple containing the GPS coordinates received from a vehicle the
     *  road ID corresponding to the position of the vehicle in the map of the city.
     *  @param t input tuple
     *  @param rc runtime context used to access to the parallelism degree and replica index
     */
    void operator()(tuple_t& t, Shipper<result_t>& shipper, RuntimeContext rc) {
        if (processed == 0) {
            parallelism = rc.getParallelism();
            replica_id = rc.getReplicaIndex();
        }
        //print_tuple("[MapMatcher] Received tuple: ", t);

        if (t.speed >= 0 &&
            t.longitude <= max_lon && t.longitude >= min_lon &&
            t.latitude <= max_lat && t.latitude >= min_lat)
        {
            OGRPoint p(t.longitude, t.latitude);
            int road_id = road_grid_list.fetch_road_ID(p);
            if (road_id != -1) {
                // road_id keys
                if (key_occ.find(road_id) == key_occ.end())
                    key_occ.insert(make_pair(road_id, 0));

                result_t r;
                r.speed = t.speed;
                r.key = road_id;
                r.id = key_occ.at(road_id)++;
                r.ts = t.ts;
                shipper.push(r);
                emitted++;

                print_result("[MapMatcher] Result: ", t)
            }
            valid_points++;
        }
        processed++;
        current_time = current_time_usecs();
    }

    ~Map_Matcher_Functor() {
        if (processed != 0) {
            cout << "[MapMatcher] replica " << replica_id + 1 << "/" << parallelism
                 << ", execution time: " << (current_time - start_time) / 1000000L
                 << " s, processed: " << processed
                 << ", valid: " << valid_points
                 << ", emitted: " << emitted
                 << ", bandwidth: " << processed / ((current_time - start_time) / 1000000L)
                 << ", #keys: " << key_occ.size()
                 << endl

            // print road_id keys and number of occurrences
                 << ", keys: "
                 << endl;
            for (auto k : key_occ) {
                cout << "key: " << k.first << " id: " << k.second << endl;
            }
        }
    }
};

#endif //TRAFFICMONITORING_MAP_MATCHER_HPP
