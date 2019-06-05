#!/usr/bin/env bash

# @author   Alessandra Fais
# @date     05/06/2019

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

NSOURCE_MAX=16
for nsource in $(seq 1 $NSOURCE_MAX);
do
    NDET_MAX=$((NTHREADS-nsource))
    for ndet in $(seq 1 $NDET_MAX);
    do
        printf "\ntest_map --nsource $nsource --naverage $ndet --ndetector $ndet --nsink $ndet\n\n"

        ./main_map --file ~/data/app/sd/sensors.dat --nsource $nsource --naverage $ndet --ndetector $ndet --nsink $ndet --rate -1 | tee ../tests/output_fd60s_map_bounded/main_$nsource-$ndet.log
    done
done

############################################### extract results ########################################################

printf "Extracting bandwidth and latency values\n"

#for filename in ../tests/output_fd60s_map_bounded/*.log; do
#    if [ ! -e "$filename" ]; then continue; fi
#    grep "\[SUMMARY\] bandwidth" "$filename" | awk  -F'[, ]' '{ print $4 }' | tee ../tests/output_fd60s_map_bounded/$(basename "$filename" .log)_bw.txt
#    grep "\[SUMMARY\] average latency" "$filename" | awk  -F'[, ]' '{ print $5 }' | tee ../tests/output_fd60s_map_bounded/$(basename "$filename" .log)_latency.txt
#done

for filename in ../tests/output_fd60s_map_bounded/*.log; do
    if [ ! -e "$filename" ]; then continue; fi
    grep "\[SUMMARY\] bandwidth" "$filename" | awk  -F'[, ]' '{ print $4 }' >> ../tests/output_fd60s_map_bounded/bandwidth.txt
    grep "\[SUMMARY\] average latency" "$filename" | awk  -F'[, ]' '{ print $5 }' >> ../tests/output_fd60s_map_bounded/latency.txt
done