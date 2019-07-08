#!/usr/bin/env bash

# @author   Alessandra Fais
# @date     08/07/2019

############################################### extract results ########################################################

printf "Extracting bandwidth and latency values for tests map, bounded buffer\n"

NTHREADS=32

NSOURCE_MAX=6
for nsource in $(seq 1 $NSOURCE_MAX);
do
    NCOUNT_MAX=$((NTHREADS-nsource))
    for ncount in $(seq 1 $NCOUNT_MAX);
    do
        printf "extract from tests/output_fd60s_map_bounded/main_$nsource-$nsource-$ncount-1.log\n\n"

        grep "\[SUMMARY\] bandwidth" tests/output_60s_bounded/main_$nsource-$nsource-$ncount-1.log | awk  -F'[, ]' '{ print $5 }' >> tests/output_60s_bounded/bandwidth.txt
	    grep "\[SUMMARY\] average latency" tests/output_60s_bounded/main_$nsource-$nsource-$ncount-1.log | awk  -F'[, ]' '{ print $6 }' >> tests/output_60s_bounded/latency.txt
    done
done