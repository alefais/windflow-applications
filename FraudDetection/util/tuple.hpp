/**
 *  @file    tuple.hpp
 *  @author  Alessandra Fais
 *  @date    04/05/2019
 *
 *  @brief Structure of a input tuple
 *
 *  This file defines the structure of the tuples generated by the source.
 *  The data type tuple_t must be default constructible, with a copy constructor
 *  and copy assignment operator, and it must provide and implement the setInfo() and
 *  getInfo() methods.
 */

#ifndef FRAUDDETECTION_TUPLE_HPP
#define FRAUDDETECTION_TUPLE_HPP

#include <windflow.hpp>

using namespace std;

struct tuple_t {
    string entity_id;   // identifies the customer
    string record;      // contains the transaction id and the transaction type
    size_t key;         // key
    uint64_t id;        // id
    uint64_t ts;        // timestamp

    // default constructor
    tuple_t() : entity_id(""), record(""), key(0), id(0), ts(0) {}

    // constructor
    tuple_t(string _entity_id, string _record, size_t _key, uint64_t _id, uint64_t _ts) :
        entity_id(_entity_id), record(_record), key(_key), id(_id), ts(_ts) {}

    // getInfo method
    tuple<size_t, uint64_t, uint64_t> getInfo() const {
        return tuple<size_t, uint64_t, uint64_t>(key, id, ts);
    }

    // setInfo method
    void setInfo(size_t _key, uint64_t _id, uint64_t _ts) {
        key = _key;
        id = _id;
        ts = _ts;
    }

    ~tuple_t() {}
};

#endif //FRAUDDETECTION_TUPLE_HPP
