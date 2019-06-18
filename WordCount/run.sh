#!/usr/bin/env bash

# @author   Alessandra Fais
# @date     18/06/2019

############################################## create test directories #################################################

if [ ! -d tests ]; then
    mkdir tests
fi
if [ ! -d tests/output_fd60s_map_bounded ]; then
    mkdir tests/output_fd60s_map_bounded
fi

#################################################### run tests #########################################################

cd bin

printf "Running tests map bounded buffer\n"

NCORES=16
NTHREADS=32

NSOURCE_MAX=10
for nsource in $(seq 1 $NSOURCE_MAX);
do
    NSPLIT_MAX=$((NTHREADS-nsource-nsource))
    for nsplit in $(seq 1 $NSPLIT_MAX);
    do
        printf "\ntest_map --nsource $nsource --nsplitter $nsplit --ncounter $nsource --nsink $nsource\n\n"

        ./main --nsource $nsource --nsplitter $nsplit --ncounter $nsource --nsink $nsource --rate -1 | tee ../tests/output_fd60s_map_bounded/main_$nsource-$nsplit-$nsource.log
    done
done

cd ..