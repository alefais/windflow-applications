#!/usr/bin/env bash

# @author   Alessandra Fais
# @date     09/07/2019

############################################ execute WindFlow FD tests #####################################################

./run_chained.sh
./extract_results_chained.sh

./run_unchained.sh
./extract_results_unchained.sh