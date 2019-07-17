#!/usr/bin/env bash

# @author   Alessandra Fais
# @date     17/07/2019

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

printf "${GREEN}Running WindFlow tests for FraudDetection application with bounded buffer and chaining.\n${NORMAL}"

NTHREADS=32
NSOURCE_MAX=5
for nsource in $(seq 1 $NSOURCE_MAX);
do
    NPRED_MAX=$((NTHREADS-nsource))
    for npred in {0..32..2};
    do
        if [ $npred -eq 0 ];
        then
            printf "${BLUE}windflow_frauddetection --nsource $nsource --npred 1 --nsink 1 --rate -1\n\n${NORMAL}"

            ./main_light_source_chained --nsource $nsource --npredictor 1 --nsink 1 --rate -1 | tee ../tests/output_60s_bounded_chained/main_$nsource-1-1_-1.log

        elif [ $npred -le $NPRED_MAX ];
        then
            printf "${BLUE}windflow_frauddetection --nsource $nsource --npred $npred --nsink $npred --rate -1\n\n${NORMAL}"

            ./main_light_source_chained --nsource $nsource --npredictor $npred --nsink $npred --rate -1 | tee ../tests/output_60s_bounded_chained/main_$nsource-$npred-$(npred)_-1.log
        fi
    done
done

cd ..