/**
 *  @file    test_markov_model.cpp
 *  @author  Alessandra Fais
 *  @date    05/05/2019
 *
 *  @brief test for Markov_Model class
 */

#include <iostream>
#include <ff/ff.hpp>
#include "util/constants.hpp"
#include "markov_model_prediction/markov_model.hpp"

using namespace std;

// /usr/local/gcc-7.2.0/bin/g++ -std=c++17 -I $HOME/fastflow -I $HOME/WindFlow/includes -I . -O3 -pthread -o test_mm test_markov_model.cpp

int main(int argc, char* argv[]) {
    cout << "Test Markov_Model class correctness" << endl;

    Markov_Model mm(_model_file);

    cout << "The model has " << mm.get_num_states() << " states." << endl;
    cout << "The states are: " << endl;
    for (string state : mm.get_states()) {
        cout << state << " ";
    }
    cout << endl;

    // this search fails...
    auto it = find(mm.get_states().begin(), mm.get_states().end(), "MNN");
    if (it != mm.get_states().end()) cout << "Element MNN found!" << endl;
    ptrdiff_t idx = distance(mm.get_states().begin(), it);
    cout << "Print index of element MNN, it is " << idx << endl;

    // this search is exact!
    size_t index = mm.get_index_of("MNN");
    if (index != -1)
        cout << "Element MNN found! Index is " << index << endl;

    cout << "The state transition probability matrix is: " << endl;
    double** matrix = mm.get_state_trans_prob();
    for (int i = 0; i < mm.get_num_states(); i++) {
        for (int j = 0; j < mm.get_num_states(); j++) { // print row
            cout << matrix[i][j] << "  ";
        }
        cout << endl;
    }

    cout << "Test Markov_Model ended" << endl;
    return 0;
}