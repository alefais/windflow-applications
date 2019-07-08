#!/usr/bin/env bash

# @author   Alessandra Fais
# @date     08/07/2019

############################################## create test directories #################################################

if [ ! -d tests ]; then
    mkdir tests
fi
if [ ! -d tests/output_60s_map_bounded ]; then
    mkdir tests/output_60s_map_bounded
fi

#################################################### run tests #########################################################

cd bin

printf "Running tests map bounded buffer\n"

NCORES=16
NTHREADS=32

NSOURCE_MAX=1
for nsource in $(seq 1 $NSOURCE_MAX);
do
    NMATCH_MAX=$((NTHREADS-nsource-1))
    for nmatch in $(seq 1 $NMATCH_MAX);
    do
        printf "\ntest_map --nsource $nsource --nmatcher $nmatch --ncalculator 1 --nsink 1 --rate 10000\n\n"

        ./main --nsource $nsource --nmatcher $nmatch --ncalculator 1 --nsink 1 --rate 10000 | tee ../tests/output_60s_map_bounded/main_$nsource-$nmatch-1-1-10000.log
    done
done

cd ..