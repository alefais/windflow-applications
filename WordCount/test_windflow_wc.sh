#!/usr/bin/env bash

# @author   Alessandra Fais
# @date     16/07/2019

############################################ execute WindFlow WC tests #####################################################

./run_chained.sh
./extract_results_chained.sh

./run_unchained.sh
./extract_results_unchained.sh