#!/usr/bin/env bash

# @author   Alessandra Fais
# @date     08/07/2019

############################################### extract results ########################################################

printf "Extracting bandwidth and latency values for tests map bounded buffer, chained (calculator + sink)\n"

NTHREADS=32

NSOURCE_MAX=1
for nsource in $(seq 1 $NSOURCE_MAX);
do
    NMATCH_MAX=$((NTHREADS-nsource-1))
    for nmatch in $(seq 1 $NMATCH_MAX);
    do
        printf "extract from tests/output_60s_map_bounded_chained/main_$nsource-$nmatch-1-1-10000.log\n\n"

        grep "\[SUMMARY\] bandwidth" tests/output_60s_map_bounded_chained/main_$nsource-$nmatch-1-1-10000.log | awk  -F'[, ]' '{ print $3 }' >> tests/output_60s_map_bounded_chained/bandwidth.txt
	    grep "\[SUMMARY\] average latency" tests/output_60s_map_bounded_chained/main_$nsource-$nmatch-1-1-10000.log | awk  -F'[, ]' '{ print $6 }' >> tests/output_60s_map_bounded_chained/latency.txt
    done
done