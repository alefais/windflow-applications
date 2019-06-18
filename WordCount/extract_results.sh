#!/usr/bin/env bash

# @author   Alessandra Fais
# @date     18/06/2019

############################################### extract results ########################################################

printf "Extracting bandwidth and latency values\n"

NTHREADS=32

NSOURCE_MAX=10
for nsource in $(seq 1 $NSOURCE_MAX);
do
    NSPLIT_MAX=$((NTHREADS-nsource-nsource))
    for nsplit in $(seq 1 $NSPLIT_MAX);
    do
        printf "extract from tests/output_fd60s_map_bounded/main_$nsource-$nsplit-$nsource.log\n\n"

        grep "\[SUMMARY\] bandwidth" tests/output_fd60s_map_bounded/main_$nsource-$nsplit-$nsource.log | awk  -F'[, ]' '{ print $5 }' >> tests/output_fd60s_map_bounded/bandwidth.txt
	    grep "\[SUMMARY\] average latency" tests/output_fd60s_map_bounded/main_$nsource-$nsplit-$nsource.log | awk  -F'[, ]' '{ print $6 }' >> tests/output_fd60s_map_bounded/latency.txt
    done
done