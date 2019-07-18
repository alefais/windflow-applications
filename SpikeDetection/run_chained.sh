#!/usr/bin/env bash

# @author   Alessandra Fais
# @date     18/07/2019

############################################## create test directories #################################################

if [ ! -d tests ]; then
    mkdir tests
fi
if [ ! -d tests/output_60s_bounded_chained ]; then
    mkdir tests/output_60s_bounded_chained
fi

#################################################### run tests #########################################################

cd bin

RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
BLUE=$(tput setaf 4)
MAGENTA=$(tput setaf 5)
CYAN=$(tput setaf 6)
NORMAL=$(tput sgr0)

printf "${GREEN}Running WindFlow tests for SpikeDetection application with bounded buffer and chaining.\n${NORMAL}"

NTHREADS=32
NSOURCE_MAX=5
for nsource in $(seq 1 $NSOURCE_MAX);
do
    NDET_MAX=$((NTHREADS-nsource))
    for ndet in {0..32..2};
    do
        if [ $ndet -eq 0 ];
        then
            printf "${BLUE}windflow_spikedetection --nsource $nsource --naverage $nsource --ndetector $nsource --nsink 1 --rate -1\n\n${NORMAL}"

            ./main_chained --nsource $nsource --naverage $nsource --ndetector $nsource --nsink 1 --rate -1 | tee ../tests/output_60s_bounded_chained/main_$nsource-$nsource-$nsource-1_-1.log

        elif [ $ndet -le $NDET_MAX ];
        then
            printf "${BLUE}windflow_spikedetection --nsource $nsource --naverage $ndet --ndetector $ndet --nsink $ndet --rate -1\n\n${NORMAL}"

            ./main_chained --nsource $nsource --naverage $ndet --ndetector $ndet --nsink $ndet --rate -1 | tee ../tests/output_60s_bounded_chained/main_$nsource-$ndet-$ndet-$(ndet)_-1.log
        fi
    done
done

cd ..
