#!/usr/bin/env bash

# @author   Alessandra Fais
# @date     08/07/2019

############################################### extract results ########################################################

printf "Extracting bandwidth and latency values for tests map, bounded buffer, chained (source + splitter and counter + sink)\n"

NTHREADS=32

NSOURCE_MAX=6
for nsource in $(seq 1 $NSOURCE_MAX);
do
    NCOUNT_MAX=$((NTHREADS-nsource))
    for ncount in $(seq 1 $NCOUNT_MAX);
    do
        printf "extract from tests/output_fd60s_map_bounded_chained/main_$nsource-$nsource-$ncount-$ncount.log\n\n"

        grep "\[SUMMARY\] bandwidth" tests/output_60s_bounded_chained/main_$nsource-$nsource-$ncount-$ncount.logg | awk  -F'[, ]' '{ print $5 }' >> tests/output_60s_bounded_chained/bandwidth.txt
	    grep "\[SUMMARY\] average latency" tests/output_60s_bounded_chained/main_$nsource-$nsource-$ncount-$ncount.log | awk  -F'[, ]' '{ print $6 }' >> tests/output_60s_bounded_chained/latency.txt
    done
done