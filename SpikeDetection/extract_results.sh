#!/usr/bin/env bash

# @author   Alessandra Fais
# @date     18/06/2019

############################################### extract results ########################################################

printf "Extracting bandwidth and latency values\n"

NTHREADS=32

NSOURCE_MAX=8
for nsource in $(seq 1 $NSOURCE_MAX);
do
    NDET_MAX=$((NTHREADS-nsource))
    for ndet in $(seq 1 $NDET_MAX);
    do
        printf "extract from tests/output_fd60s_map_bounded/main_$nsource-$ndet.log\n\n"

        grep "\[SUMMARY\] bandwidth" tests/output_fd60s_map_bounded/main_$nsource-$ndet.log | awk  -F'[, ]' '{ print $4 }' >> tests/output_fd60s_map_bounded/bandwidth.txt
	    grep "\[SUMMARY\] average latency" tests/output_fd60s_map_bounded/main_$nsource-$ndet.log | awk  -F'[, ]' '{ print $7 }' >> tests/output_fd60s_map_bounded/latency.txt
    done
done