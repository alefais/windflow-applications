#!/usr/bin/env bash

# @author   Alessandra Fais
# @date     16/07/2019

############################################### extract results ########################################################

RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
BLUE=$(tput setaf 4)
MAGENTA=$(tput setaf 5)
CYAN=$(tput setaf 6)
NORMAL=$(tput sgr0)

printf "${GREEN}Extracting bandwidth and latency values for WordCount application\n${NORMAL}"

NTHREADS=32
NSOURCE_MAX=5
for nsource in $(seq 1 $NSOURCE_MAX);
do
    NCOUNT_MAX=$((NTHREADS-nsource))
    for ncount in {0..32..2};
    do
        if [ $ncount -eq 0 ];
        then
            printf "${BLUE}extract from tests/output_60s_bounded_chained/main_$nsource-$nsource-1-1_-1.log\n\n${NORMAL}"

            # bandwidth
            grep "\[SUMMARY\] bandwidth" tests/output_60s_bounded_chained/main_$nsource-$nsource-1-1_-1.log | awk  -F'[, ]' '{ print $5 }' >> tests/output_60s_bounded_chained/bandwidth.txt

	        # latency
	        grep "\[Sink\] latency" tests/output_60s_bounded_chained/main_$nsource-$nsource-1-1_-1.log | awk  -F'[, ]' '{ print $4 " " $6 " " $8 " " $10 " " $12 " " $14 " " $16 " " $18 }' >> tests/output_60s_bounded_chained/latency.txt
	        grep "\[Sink\] latency" tests/output_60s_bounded_chained/main_$nsource-$nsource-1-1_-1.log | awk  -F'[, ]' '{ print $4 }' >> tests/output_60s_bounded_chained/latency_mean.txt

        elif [ $ncount -le $NCOUNT_MAX ];
        then
            printf "${BLUE}extract from tests/output_60s_bounded_chained/main_$nsource-$nsource-$ncount-$ncount_-1.log\n\n${NORMAL}"

            # bandwidth
            grep "\[SUMMARY\] bandwidth" tests/output_60s_bounded_chained/main_$nsource-$nsource-$ncount-$(ncount)_-1.log | awk  -F'[, ]' '{ print $5 }' >> tests/output_60s_bounded_chained/bandwidth.txt

	        # latency
	        grep "\[Sink\] latency" tests/output_60s_bounded_chained/main_$nsource-$nsource-$ncount-$(ncount)_-1.log | awk  -F'[, ]' '{ print $4 " " $6 " " $8 " " $10 " " $12 " " $14 " " $16 " " $18 }' >> tests/output_60s_bounded_chained/latency.txt
	        grep "\[Sink\] latency" tests/output_60s_bounded_chained/main_$nsource-$nsource-$ncount-$(ncount)_-1.log | awk  -F'[, ]' '{ print $4 }' >> tests/output_60s_bounded_chained/latency_mean.txt
        fi
    done
done

if [ ! -d tests/output_60s_bounded_chained/logs ]; then
    mkdir tests/output_60s_bounded_chained/logs
fi

mv tests/output_60s_bounded_chained/*.log tests/output_60s_bounded_chained/logs/

if [ ! -d tests/output_60s_bounded_chained/statistics ]; then
    mkdir tests/output_60s_bounded_chained/statistics
fi

mv tests/output_60s_bounded_chained/*.txt tests/output_60s_bounded_chained/statistics/