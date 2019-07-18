#!/usr/bin/env bash

# @author   Alessandra Fais
# @date     18/07/2019

############################################### extract results ########################################################

RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
BLUE=$(tput setaf 4)
MAGENTA=$(tput setaf 5)
CYAN=$(tput setaf 6)
NORMAL=$(tput sgr0)

NTHREADS=32
NSOURCE_MAX=5
for nsource in $(seq 1 $NSOURCE_MAX);
do
    for navg in {0..8..2};
    do
        if [ $navg -eq 0 ];
        then
            printf "${BLUE}extract from tests/output_60s_bounded_unchained/main_$nsource-$nsource-$nsource-1_-1.log\n\n${NORMAL}"

            # bandwidth
            grep "\[SUMMARY\] bandwidth" tests/output_60s_bounded_unchained/main_$nsource-$nsource-$nsource-1_-1.log | awk  -F'[, ]' '{ print $3 }' >> tests/output_60s_bounded_unchained/bandwidth.txt

	        # latency
	        grep "\[Sink\] latency" tests/output_60s_bounded_unchained/main_$nsource-$nsource-$nsource-1_-1.log | awk  -F'[, ]' '{ print $4 " " $6 " " $8 " " $10 " " $12 " " $14 " " $16 " " $18 }' >> tests/output_60s_bounded_unchained/latency.txt
	        grep "\[Sink\] latency" tests/output_60s_bounded_unchained/main_$nsource-$nsource-$nsource-1_-1.log | awk  -F'[, ]' '{ print $4 }' >> tests/output_60s_bounded_unchained/latency_mean.txt

        elif [ $navg -gt $nsource ];
        then
            for ndet in {2..8..2};
            do
                if [ $ndet -ge $navg ];
                then
                    printf "${BLUE}extract from tests/output_60s_bounded_unchained/main_$nsource-$navg-$ndet-1_-1.log\n\n${NORMAL}"

                    # bandwidth
                    grep "\[SUMMARY\] bandwidth" tests/output_60s_bounded_unchained/main_$nsource-$navg-$ndet-1_-1.log | awk  -F'[, ]' '{ print $3 }' >> tests/output_60s_bounded_unchained/bandwidth.txt

                    # latency
                    grep "\[Sink\] latency" tests/output_60s_bounded_unchained/main_$nsource-$navg-$ndet-1_-1.log | awk  -F'[, ]' '{ print $4 " " $6 " " $8 " " $10 " " $12 " " $14 " " $16 " " $18 }' >> tests/output_60s_bounded_unchained/latency.txt
                    grep "\[Sink\] latency" tests/output_60s_bounded_unchained/main_$nsource-$navg-$ndet-1_-1.log | awk  -F'[, ]' '{ print $4 }' >> tests/output_60s_bounded_unchained/latency_mean.txt
                fi
            done
        fi
    done
done

if [ ! -d tests/output_60s_bounded_unchained/logs ]; then
    mkdir tests/output_60s_bounded_unchained/logs
fi

mv tests/output_60s_bounded_unchained/*.log tests/output_60s_bounded_unchained/logs/

if [ ! -d tests/output_60s_bounded_unchained/statistics ]; then
    mkdir tests/output_60s_bounded_unchained/statistics
fi

mv tests/output_60s_bounded_unchained/*.txt tests/output_60s_bounded_unchained/statistics/