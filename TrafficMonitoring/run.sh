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

NSOURCE_MAX=5
for nsource in $(seq 1 $NSOURCE_MAX);
do
    NMATCH_MAX=$((NTHREADS-nsource-nsource))
    for nmatch in $(seq 1 $NMATCH_MAX);
    do
        printf "\ntest_map --nsource $nsource --nmatcher $nmatch --ncalculator $nsource --nsink $nsource\n\n"

        ./main --nsource $nsource --nmatcher $nmatch --ncalculator $nsource --nsink $nsource --rate -1 | tee ../tests/output_fd60s_map_bounded/main_$nsource-$nmatch-$nsource.log
    done
done

cd ..