#!/usr/bin/env bash

# @author   Alessandra Fais
# @date     16/07/2019

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

printf "${GREEN}Running WindFlow tests for WordCount application with bounded buffer and chaining.\n${NORMAL}"

NTHREADS=32
NSOURCE_MAX=5
for nsource in $(seq 1 $NSOURCE_MAX);
do
    NCOUNT_MAX=$((NTHREADS-nsource))
    for ncount in {0..32..2};
    do
        if [ $ncount -eq 0 ];
        then
            printf "${BLUE}windflow_wordcount --nsource $nsource --nsplitter $nsource --ncounter 1 --nsink 1 --rate -1\n\n${NORMAL}"

            ./main --nsource $nsource --nsplitter $nsource --ncounter 1 --nsink 1 --rate -1 | tee ../tests/output_60s_bounded_chained/main_$nsource-$nsource-1-1_-1.log

        elif [ $ncount -le $NCOUNT_MAX ];
        then
            printf "${BLUE}windflow_wordcount --nsource $nsource --nsplitter $nsource --ncounter $ncount --nsink $ncount --rate -1\n\n${NORMAL}"

            ./main --nsource $nsource --nsplitter $nsource --ncounter $ncount --nsink $ncount --rate -1 | tee ../tests/output_60s_bounded_chained/main_$nsource-$nsource-$ncount-$ncount_-1.log
        fi
    done
done

cd ..