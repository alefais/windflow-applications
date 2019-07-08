#!/usr/bin/env bash

# @author   Alessandra Fais
# @date     08/07/2019

############################################## create test directories #################################################

if [ ! -d tests ]; then
    mkdir tests
fi
if [ ! -d tests/output_60s_light_bounded_chained ]; then
    mkdir tests/output_60s_light_bounded_chained
fi

#################################################### run tests #########################################################

cd bin

printf "Running tests light source, bounded buffer, chained (predictor + sink)\n"

NCORES=16
NTHREADS=32

NSOURCE_MAX=5
for nsource in $(seq 1 $NSOURCE_MAX);
do
    NPRED_MAX=$((NTHREADS-nsource))
    for npred in $(seq 1 $NPRED_MAX);
    do
        printf "\ntest_light_source --nsource $nsource --npred $npred --nsink $npred\n\n"

        ./main_light_source --nsource $nsource --npredictor $npred --nsink $npred --rate -1 | tee ../tests/output_60s_light_bounded_chained/main_$nsource-$npred-$npred.log
    done
done