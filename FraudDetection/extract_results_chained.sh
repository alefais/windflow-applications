#!/usr/bin/env bash

# @author   Alessandra Fais
# @date     08/07/2019

############################################### extract results ########################################################

printf "Extracting bandwidth and latency values for tests light source, bounded buffer, chained (predictor + sink)\n"

NTHREADS=32

NSOURCE_MAX=5
for nsource in $(seq 1 $NSOURCE_MAX);
do
    NPRED_MAX=$((NTHREADS-nsource))
    for npred in $(seq 1 $NPRED_MAX);
    do
        printf "extract from tests/output_60s_light_bounded_chained/main_$nsource-$npred-$npred.log\n\n"

        grep "\[SUMMARY\] bandwidth" tests/output_60s_light_bounded_chained/main_$nsource-$npred-$npred.log | awk  -F'[, ]' '{ print $4 }' >> tests/output_60s_light_bounded_chained/bandwidth.txt
	    grep "\[SUMMARY\] average latency" tests/output_60s_light_bounded_chained/main_$nsource-$npred-$npred.log | awk  -F'[, ]' '{ print $7 }' >> tests/output_60s_light_bounded_chained/latency.txt
    done
done