/**
 *  @file    atomic_double.hpp
 *  @author  Alessandra Fais
 *  @date    04/06/2019
 *
 *  @brief Class implementing atomic fetch and add operation on a double
 *
 *  The class is used to evaluate the average value of the latency in the system.
 *  All the replicas of the sink node update this value adding the local average
 *  latency value computed. At the end, a mean of all the average values is done
 *  in order to obtain the total average latency of the system.
 */

#ifndef TRAFFICMONITORING_ATOMIC_DOUBLE_HPP
#define TRAFFICMONITORING_ATOMIC_DOUBLE_HPP

#include <ff/ff.hpp>
#include <windflow.hpp>

using namespace std;

class Atomic_Double {
private:
    double average_latency;    // sum of the average latency values measured in each of the sink's replicas
    mutex m;                   // mutex

public:
    Atomic_Double() : average_latency(0) {}

    double fetch_add(double value) {
        unique_lock<mutex> lock(m);
        double old_value = average_latency;
        average_latency = average_latency + value;

        return old_value;
    }

    double get() {
        unique_lock<mutex> lock(m);
        return average_latency;
    }

    ~Atomic_Double() {}
};

#endif //TRAFFICMONITORING_ATOMIC_DOUBLE_HPP
