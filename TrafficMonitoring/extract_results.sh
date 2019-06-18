#!/usr/bin/env bash

# @author   Alessandra Fais
# @date     18/06/2019

############################################### extract results ########################################################

printf "Extracting bandwidth and latency values\n"

NTHREADS=32

NSOURCE_MAX=5
for nsource in $(seq 1 $NSOURCE_MAX);
do
    NMATCH_MAX=$((NTHREADS-nsource-nsource))
    for nmatch in $(seq 1 $NMATCH_MAX);
    do
        printf "extract from tests/output_60s_map_bounded/main_$nsource-$nmatch-$nsource.log\n\n"

        grep "\[SUMMARY\] bandwidth" tests/output_60s_map_bounded/main_$nsource-$nmatch-$nsource.log | awk  -F'[, ]' '{ print $3 }' >> tests/output_60s_map_bounded/bandwidth.txt
	    grep "\[SUMMARY\] average latency" tests/output_60s_map_bounded/main_$nsource-$nmatch-$nsource.log | awk  -F'[, ]' '{ print $6 }' >> tests/output_60s_map_bounded/latency.txt
    done
done