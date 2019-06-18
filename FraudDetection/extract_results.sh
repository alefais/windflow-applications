#!/usr/bin/env bash

# @author   Alessandra Fais
# @date     18/06/2019

############################################### extract results ########################################################

printf "Extracting bandwidth and latency values\n"

NTHREADS=32

NSOURCE_MAX=5
for nsource in $(seq 1 $NSOURCE_MAX);
do
    NPRED_MAX=$((NTHREADS-nsource))
    for npred in $(seq 1 $NPRED_MAX);
    do
        printf "extract from tests/output_60s_light_bounded/main_$nsource-$npred.log\n\n"

        grep "\[SUMMARY\] bandwidth" tests/output_60s_light_bounded/main_$nsource-$npred.log | awk  -F'[, ]' '{ print $4 }' >> tests/output_60s_light_bounded/bandwidth.txt
	    grep "\[SUMMARY\] average latency" tests/output_60s_light_bounded/main_$nsource-$npred.log | awk  -F'[, ]' '{ print $7 }' >> tests/output_60s_light_bounded/latency.txt
    done
done