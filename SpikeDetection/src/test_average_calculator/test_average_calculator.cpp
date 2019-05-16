/**
*  @file    test_average_calculator.cpp
*  @author  Alessandra Fais
*  @date    16/05/2019
*
*  @brief test for Average_Calculator_Functor class
*/

#include <iostream>
#include <ff/ff.hpp>
#include <windflow.hpp>
#include "../../includes/nodes/average_calculator_map.hpp"
#include "../../includes/util/cli_util.hpp"

using namespace std;

// generate a stream of integers
class Generator_Functor {
private:
    size_t stream_len;     // stream length per key
    size_t keys;           // number of keys

public:
    // constructor
    Generator_Functor(size_t _stream_len, size_t _keys): stream_len(_stream_len), keys(_keys) {}

    void operator()(Shipper<tuple_t>& shipper) {
        // generation of the input stream
        for (size_t i = 0; i < stream_len; i++) {
            for (size_t k = 0; k < keys; k++) {
                tuple_t t(i, 0, k, i, 0);
                shipper.push(t);
                //cout << "[Source] sent " << t.property_value << " " << t.key << " " << t.id << endl;
            }
        }
    }

    // destructor
    ~Generator_Functor() {}
};

// receive a stream of results
class Sink_Functor {
private:
    size_t received;

public:
    // constructor
    Sink_Functor() : received(0) {}

    void operator()(optional<tuple_t>& t) {
        if (t) {
            received++;
            /*cout << "[Sink] received " << t->property_value
                 << " " << t->incremental_average
                 << ", " << t->key
                 << " " << t->id << endl;*/
        } else
            cout << "[Sink] received " << received << " results" << endl;
    }

    // destructor
    ~Sink_Functor() {}
};

int main(int argc, char* argv[]) {
    cout << "Test Average_Calculator_Map_Functor class correctness" << endl;

    Generator_Functor generator_functor(100, 10);
    Source source = Source_Builder(generator_functor)
                .withParallelism(1)
                .withName(source_name)
                .build();

    Average_Calculator_Map_Functor avg_functor;
    Map map = Map_Builder(avg_functor)
                .withParallelism(2)
                .withName(avg_calc_name)
                .enable_KeyBy()
                .build();

    Sink_Functor sink_functor;
    Sink sink = Sink_Builder(sink_functor)
                .withParallelism(1)
                .withName(sink_name)
                .build();

    MultiPipe topology(topology_name);
    topology.add_source(source);
    topology.add(map);
    topology.add_sink(sink);
    if (topology.run_and_wait_end() < 0)
        cerr << app_error << endl;
    else
        cout << app_termination << topology.cardinality() << endl;

    cout << "Test Average_Calculator_Map_Functor ended" << endl;
    return 0;
}
