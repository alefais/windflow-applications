/**
 *  @file    test_model_predictor.cpp
 *  @author  Alessandra Fais
 *  @date    05/05/2019
 *
 *  @brief test for Markov_Model_Predictor class
 */

#include <iostream>
#include <ff/ff.hpp>
#include "util/constants.hpp"
#include "markov_model_prediction/model_based_predictor.hpp"

using namespace std;

// /usr/local/gcc-7.2.0/bin/g++ -std=c++17 -I $HOME/fastflow -I $HOME/WindFlow/includes -I . -O3 -pthread -o test_mmp test_model_predictor.cpp

int main(int argc, char* argv[]) {
    cout << "Test Markov_Model_Predictor class correctness" << endl;

    Markov_Model_Predictor mmp;

    cout << "The model has been created." << endl;

    if (mmp.get_model_alg() == 0) {
        cout << "Model algorithm is miss probability." << endl;
    } else if (mmp.get_model_alg() == 1) {
        cout << "Model algorithm is miss rate." << endl;

        cout << "The maximum state probability index for each state is:" << endl;
        int *max_state_prob_idx = mmp.get_max_state_prob_idx();
        for (int s = 0; s < mmp.get_num_states(); s++) {
            cout << s << " -> " << max_state_prob_idx[s] << endl;
        }
        cout << endl;
    } else if (mmp.get_model_alg() == 2) {
        cout << "Model algorithm is entropy reduction." << endl;

        cout << "The entropy for each state is: " << endl;
        double *entropy = mmp.get_entropy();
        for (int s = 0; s < mmp.get_num_states(); s++) {
            cout << s << " -> " << entropy[s] << endl;
        }
        cout << endl;
    }

    cout << "Start test execute method..." << endl;
    mmp.execute("I8PFKM0Q49", "BFQ1I2YYX3OF,MNN", ","); // put five entries relative to this entity_id (we complete the window for this entry)
    mmp.execute("Z2E821O6VB", "23UYUALXNS6M,LHL", ",");
    mmp.execute("BI33MN3U50", "4WKU9D2YRDG5,HNN", ",");
    mmp.execute("I8PFKM0Q49", "BFQ1I2YYX3OF,MNN", ",");
    mmp.execute("I8PFKM0Q49", "BFQ1I2YYX3OF,MNN", ",");
    mmp.execute("I8PFKM0Q49", "BFQ1I2YYX3OF,MNN", ",");
    mmp.execute("I8PFKM0Q49", "BFQ1I2YYX3OF,MNN", ",");

    cout << "Test Markov_Model_Predictor ended" << endl;
    return 0;
}