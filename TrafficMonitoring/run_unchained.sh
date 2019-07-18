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

printf "${GREEN}Running WindFlow tests for TrafficMonitoring application with bounded buffer and no chaining.\n${NORMAL}"

NTHREADS=32
NSOURCE_MAX=1
for nsource in $(seq 1 $NSOURCE_MAX);
do
    NMATCH_MAX=$((NTHREADS-nsource-2))
    for nmatch in {0..32..2};
    do
        if [ $nmatch -eq 0 ];
        then
            printf "\nwindflow_trafficmonitoring --nsource $nsource --nmatcher $nsource --ncalculator 1 --nsink 1 --rate 10000\n\n"

            ./main_unchained --nsource $nsource --nmatcher $nsource --ncalculator 1 --nsink 1 --rate 10000 | tee ../tests/output_60s_bounded_unchained/main_$nsource-$nsource-1-1-10000.log

        elif [[ ($nmatch -le $NMATCH_MAX) && ($nmatch -ge $nsource) ]];
        then
            printf "\nwindflow_trafficmonitoring --nsource $nsource --nmatcher $nmatch --ncalculator 1 --nsink 1 --rate 10000\n\n"

            ./main_unchained --nsource $nsource --nmatcher $nmatch --ncalculator 1 --nsink 1 --rate 10000 | tee ../tests/output_60s_bounded_unchained/main_$nsource-$nmatch-1-1-10000.log
        fi
    done
done

cd ..