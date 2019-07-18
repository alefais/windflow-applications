#!/usr/bin/env bash

# @author   Alessandra Fais
# @date     18/07/2019

############################################## create test directories #################################################

if [ ! -d tests ]; then
    mkdir tests
fi
if [ ! -d tests/output_60s_bounded_unchained ]; then
    mkdir tests/output_60s_bounded_unchained
fi

#################################################### run tests #########################################################

cd bin

RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
BLUE=$(tput setaf 4)
MAGENTA=$(tput setaf 5)
CYAN=$(tput setaf 6)
NORMAL=$(tput sgr0)

printf "${GREEN}Running WindFlow tests for SpikeDetection application with bounded buffer and no chaining.\n${NORMAL}"

NTHREADS=32
NSOURCE_MAX=5
for nsource in $(seq 1 $NSOURCE_MAX);
do
    for navg in {0..8..2};
    do
        if [ $navg -eq 0 ];
        then
            printf "${BLUE}windflow_spikedetection --nsource $nsource --naverage $nsource --ndetector $nsource --nsink 1 --rate -1\n\n${NORMAL}"

            ./main_unchained --nsource $nsource --naverage $nsource --ndetector $nsource --nsink 1 --rate -1 | tee ../tests/output_60s_bounded_unchained/main_$nsource-$nsource-$nsource-1_-1.log

        elif [ $navg -gt $nsource ];
        then
            for ndet in {2..8..2};
            do
                if [ $ndet -ge $navg ];
                then
                    printf "${BLUE}windflow_spikedetection --nsource $nsource --naverage $navg --ndetector $ndet --nsink 1 --rate -1\n\n${NORMAL}"

                    ./main_unchained --nsource $nsource --naverage $navg --ndetector $ndet --nsink 1 --rate -1 | tee ../tests/output_60s_bounded_unchained/main_$nsource-$navg-$ndet-1_-1.log
                fi
            done
        fi
    done
done

cd ..
