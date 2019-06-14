/**
 *  @file    result.hpp
 *  @author  Alessandra Fais
 *  @date    14/06/2019
 *
 *  @brief Structure of a tuple result of the FlatMap
 *
 *  This file defines the structure of the tuples generated by the source.
 *  The data type tuple_t must be default constructible, with a copy constructor
 *  and copy assignment operator, and it must provide and implement the setInfo() and
 *  getInfo() methods.
 */

#ifndef TRAFFICMONITORING_RESULT_HPP
#define TRAFFICMONITORING_RESULT_HPP

#include <windflow.hpp>

using namespace std;

struct result_t {
    double speed;               // vehicle speed
    size_t key;                 // road ID corresponding to latitude and longitude coordinates of the vehicle
    uint64_t id;                // id that indicates the occurrence of the key vehicle_id
    uint64_t ts;                // timestamp

    // default constructor
    result_t(): speed(0.0), key(0), id(0), ts(0) {}

    // constructor
    result_t(double _speed, size_t _key, uint64_t _id, uint64_t _ts): speed(_speed), key(_key), id(_id), ts(_ts) {}

    // getControlFields method
    tuple<size_t, uint64_t, uint64_t> getControlFields() const {
        return tuple<size_t, uint64_t, uint64_t>(key, id, ts);
    }

    // setControlFields method
    void setControlFields(size_t _key, uint64_t _id, uint64_t _ts) {
        key = _key;
        id = _id;
        ts = _ts;
    }

    // destructor
    ~result_t() {}

};

#endif //TRAFFICMONITORING_RESULT_HPP
