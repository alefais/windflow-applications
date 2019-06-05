/**
 *  @file    tuple.hpp
 *  @author  Alessandra Fais
 *  @date    04/06/2019
 *
 *  @brief Structure of a tuple
 *
 *  This file defines the structure of the tuples generated by the source.
 *  The data type tuple_t must be default constructible, with a copy constructor
 *  and copy assignment operator, and it must provide and implement the setInfo() and
 *  getInfo() methods.
 */

#ifndef TRAFFICMONITORING_TUPLE_HPP
#define TRAFFICMONITORING_TUPLE_HPP

#include <windflow.hpp>

using namespace std;

struct tuple_t {
    double latitude;            // vehicle latitude
    double longitude;           // vehicle longitude
    double speed;               // vehicle speed
    int direction;              // vehicle direction
    size_t key;                 // vehicle_id that identifies the vehicle (taxi or bus)
    uint64_t id;                // id that indicates the occurrence of the key vehicle_id
    uint64_t ts;                // timestamp

    // default constructor
    tuple_t() : latitude(0.0), longitude(0.0), speed(0.0), direction(0), key(0), id(0), ts(0) {}

    // constructor
    tuple_t(double _latitude, double _longitude, double _speed, int _direction, size_t _key, uint64_t _id, uint64_t _ts) :
        latitude(_latitude), longitude(_longitude), speed(_speed), direction(_direction), key(_key), id(_id), ts(_ts) {}

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
    ~tuple_t() {}
};

#endif //TRAFFICMONITORING_TUPLE_HPP
