#!/usr/bin/env bash

# @author   Alessandra Fais
# @date     08/07/2019

############################################## create test directories #################################################

if [ ! -d tests ]; then
    mkdir tests
fi
if [ ! -d tests/output_60s_bounded_chained ]; then
    mkdir tests/output_60s_bounded_chained
fi

#################################################### run tests #########################################################

cd bin

printf "Running tests map, bounded buffer, chained (source + splitter and counter + sink)\n"

NCORES=16
NTHREADS=32

NSOURCE_MAX=6
for nsource in $(seq 1 $NSOURCE_MAX);
do
    NCOUNT_MAX=$((NTHREADS-nsource))
    for ncount in $(seq 1 $NCOUNT_MAX);
    do
        printf "\ntest_map --nsource $nsource --nsplitter $nsource --ncounter $ncount --nsink $ncount\n\n"

        ./main --nsource $nsource --nsplitter $nsource --ncounter $ncount --nsink $ncount --rate -1 | tee ../tests/output_60s_bounded_chained/main_$nsource-$nsource-$ncount-$ncount.log
    done
done

cd ..