#!/usr/bin/env bash

# @author   Alessandra Fais
# @date     09/07/2019

############################################## create test directories #################################################

if [ ! -d tests ]; then
    mkdir tests
fi
if [ ! -d tests/output_60s_bounded_unchained ]; then
    mkdir tests/output_60s_bounded_unchained
fi

#################################################### run tests #########################################################

cd bin

printf "Running tests light source, bounded buffer, unchained\n"

NTHREADS=32
NSOURCE_MAX=5
for nsource in $(seq 1 $NSOURCE_MAX);
do
    NPRED_MAX=$((NTHREADS-nsource-1))
    for npred in $(seq 1 $NPRED_MAX);
    do
        printf "\ntest_light_source --nsource $nsource --npred $npred --nsink 1\n\n"

        ./main_light_source_unchained --nsource $nsource --npredictor $npred --nsink 1 --rate -1 | tee ../tests/output_60s_bounded_unchained/main_$nsource-$npred-1.log
    done
done

cd ..