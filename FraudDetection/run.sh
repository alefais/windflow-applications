#!/usr/bin/env bash

# @author   Alessandra Fais
# @date     04/06/2019

############################################## create test directories #################################################

if [ ! -d tests ]; then
    mkdir tests
fi
if [ ! -d tests/output_fd60s_light_bounded ]; then
    mkdir tests/output_fd60s_light_bounded
fi

#################################################### run tests #########################################################

cd bin

printf "Running tests light source bounded buffer\n"

NCORES=16
NTHREADS=32

NSOURCE_MAX=16
for nsource in $(seq 1 $NSOURCE_MAX);
do
    NPRED_MAX=$((NTHREADS-nsource))
    for npred in $(seq 1 $NPRED_MAX);
    do
        printf "\ntest_light_source --nsource $nsource --npred $npred --nsink $npred\n\n"

        ./main_light_source --file ~/data/app/fd/credit-card.dat --nsource $nsource --npredictor $npred --nsink $npred --rate -1 | tee ../tests/output_fd60s_light_bounded/main_$nsource_$npred.log
    done
done

############################################### extract results ########################################################

printf "Extracting bandwidth and latency values\n"

#for filename in tests/output_fd60s_light_bounded/*.log; do
#    if [ ! -e "$filename" ]; then continue; fi
#    grep "\[SUMMARY\] bandwidth" "$filename" | awk  -F'[, ]' '{ print $4 }' | tee tests/output_fd60s_light_bounded/$(basename "$filename" .log)_bw.txt
#	grep "\[SUMMARY\] average latency" "$filename" | awk  -F'[, ]' '{ print $5 }' | tee tests/output_fd60s_light_bounded/$(basename "$filename" .log)_latency.txt
#done

for filename in tests/output_fd60s_light_bounded/*.log; do
    if [ ! -e "$filename" ]; then continue; fi
    grep "\[SUMMARY\] bandwidth" "$filename" | awk  -F'[, ]' '{ print $4 }' >> tests/output_fd60s_light_bounded/bandwidth.txt
	grep "\[SUMMARY\] average latency" "$filename" | awk  -F'[, ]' '{ print $5 }' >> tests/output_fd60s_light_bounded/latency.txt
done