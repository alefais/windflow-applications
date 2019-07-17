#!/usr/bin/env bash

# @author   Alessandra Fais
# @date     16/07/2019

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

printf "${GREEN}Running WindFlow tests for WordCount application with bounded buffer and no chaining.\n${NORMAL}"

NTHREADS=32
NSOURCE_MAX=5
for nsource in $(seq 1 $NSOURCE_MAX);
do
    for nsplit in {0..12..2};
    do
        if [ $nsplit -eq 0 ];
        then
            printf "${BLUE}windflow_wordcount --nsource $nsource --nsplitter 1 --ncounter 1 --nsink 1 --rate -1\n\n${NORMAL}"

            ./main --nsource $nsource --nsplitter 1 --ncounter 1 --nsink 1 --rate -1 | tee ../tests/output_60s_bounded_unchained/main_$nsource-1-1-1_-1.log

            for ncount in {2..8..2};
            do
                printf "${BLUE}windflow_wordcount --nsource $nsource --nsplitter 1 --ncounter $ncount --nsink 1 --rate -1\n\n${NORMAL}"

                ./main --nsource $nsource --nsplitter 1 --ncounter $ncount --nsink 1 --rate -1 | tee ../tests/output_60s_bounded_unchained/main_$nsource-1-$ncount-1_-1.log
            done
        else
            for ncount in {2..12..2};
            do
                if [ $ncount -ge $nsplit ];
                then
                    printf "${BLUE}windflow_wordcount --nsource $nsource --nsplitter $nsplit --ncounter $ncount --nsink 1 --rate -1\n\n${NORMAL}"

                    ./main --nsource $nsource --nsplitter $nsplit --ncounter $ncount --nsink 1 --rate -1 | tee ../tests/output_60s_bounded_unchained/main_$nsource-$nsplit-$ncount-1_-1.log
                fi
            done
        fi
    done
done

cd ..