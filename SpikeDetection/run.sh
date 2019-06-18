#!/usr/bin/env bash

# @author   Alessandra Fais
# @date     18/06/2019

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

NSOURCE_MAX=8
for nsource in $(seq 1 $NSOURCE_MAX);
do
    NDET_MAX=$((NTHREADS-nsource))
    for ndet in $(seq 1 $NDET_MAX);
    do
        printf "\ntest_map --nsource $nsource --naverage $ndet --ndetector $ndet --nsink $ndet\n\n"

        ./main_map --nsource $nsource --naverage $ndet --ndetector $ndet --nsink $ndet --rate -1 | tee ../tests/output_60s_map_bounded/main_$nsource-$ndet.log
    done
done

cd ..