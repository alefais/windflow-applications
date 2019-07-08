#!/usr/bin/env bash

# @author   Alessandra Fais
# @date     08/07/2019

############################################### extract results ########################################################

printf "Extracting bandwidth and latency values for tests light source, bounded buffer\n"

NTHREADS=32

NSOURCE_MAX=5
for nsource in $(seq 1 $NSOURCE_MAX);
do
    NPRED_MAX=$((NTHREADS-nsource))
    for npred in $(seq 1 $NPRED_MAX);
    do
        printf "extract from tests/output_60s_light_bounded/main_$nsource-$npred-1.log\n\n"

        grep "\[SUMMARY\] bandwidth" tests/output_60s_light_bounded/main_$nsource-$npred-1.log | awk  -F'[, ]' '{ print $4 }' >> tests/output_60s_light_bounded/bandwidth.txt
	    grep "\[SUMMARY\] average latency" tests/output_60s_light_bounded/main_$nsource-$npred-1.log | awk  -F'[, ]' '{ print $7 }' >> tests/output_60s_light_bounded/latency.txt
    done
done