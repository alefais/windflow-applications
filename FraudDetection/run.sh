#!/usr/bin/env bash

# WindFlow tests bandwidth

#################################################################### UNBOUNDED BUFFER ####################################################################

# tests using the "heavy" source (max 16 PE - all physic cores)
./main_heavy_source_unbounded --file ~/data/app/fd/credit-card.dat --nsource 1 --npredictor 1 --nsink 1 --rate -1 | tee ../tests/output_fd60s_heavy_unbounded/main_111-1.log
./main_heavy_source_unbounded --file ~/data/app/fd/credit-card.dat --nsource 1 --npredictor 2 --nsink 1 --rate -1 | tee ../tests/output_fd60s_heavy_unbounded/main_121-1.log
./main_heavy_source_unbounded --file ~/data/app/fd/credit-card.dat --nsource 1 --npredictor 4 --nsink 1 --rate -1 | tee ../tests/output_fd60s_heavy_unbounded/main_141-1.log
./main_heavy_source_unbounded --file ~/data/app/fd/credit-card.dat --nsource 1 --npredictor 8 --nsink 1 --rate -1 | tee ../tests/output_fd60s_heavy_unbounded/main_181-1.log
./main_heavy_source_unbounded --file ~/data/app/fd/credit-card.dat --nsource 1 --npredictor 14 --nsink 1 --rate -1 | tee ../tests/output_fd60s_heavy_unbounded/main_1141-1.log

./main_heavy_source_unbounded --file ~/data/app/fd/credit-card.dat --nsource 2 --npredictor 4 --nsink 1 --rate -1 | tee ../tests/output_fd60s_heavy_unbounded/main_241-1.log
./main_heavy_source_unbounded --file ~/data/app/fd/credit-card.dat --nsource 2 --npredictor 8 --nsink 1 --rate -1 | tee ../tests/output_fd60s_heavy_unbounded/main_281-1.log
./main_heavy_source_unbounded --file ~/data/app/fd/credit-card.dat --nsource 2 --npredictor 13 --nsink 1 --rate -1 | tee ../tests/output_fd60s_heavy_unbounded/main_2131-1.log

./main_heavy_source_unbounded --file ~/data/app/fd/credit-card.dat --nsource 4 --npredictor 8 --nsink 1 --rate -1 | tee ../tests/output_fd60s_heavy_unbounded/main_481-1.log
./main_heavy_source_unbounded --file ~/data/app/fd/credit-card.dat --nsource 4 --npredictor 11 --nsink 1 --rate -1 | tee ../tests/output_fd60s_heavy_unbounded/main_4111-1.log
./main_heavy_source_unbounded --file ~/data/app/fd/credit-card.dat --nsource 5 --npredictor 10 --nsink 1 --rate -1 | tee ../tests/output_fd60s_heavy_unbounded/main_5101-1.log

# tests using the "light" source (max 16 PE - all physic cores)
./main_light_source_unbounded --file ~/data/app/fd/credit-card.dat --nsource 1 --npredictor 1 --nsink 1 --rate -1 | tee ../tests/output_fd60s_light_unbounded/main_111-1.log
./main_light_source_unbounded --file ~/data/app/fd/credit-card.dat --nsource 1 --npredictor 2 --nsink 1 --rate -1 | tee ../tests/output_fd60s_light_unbounded/main_121-1.log
./main_light_source_unbounded --file ~/data/app/fd/credit-card.dat --nsource 1 --npredictor 4 --nsink 1 --rate -1 | tee ../tests/output_fd60s_light_unbounded/main_141-1.log
./main_light_source_unbounded --file ~/data/app/fd/credit-card.dat --nsource 1 --npredictor 8 --nsink 1 --rate -1 | tee ../tests/output_fd60s_light_unbounded/main_181-1.log
./main_light_source_unbounded --file ~/data/app/fd/credit-card.dat --nsource 1 --npredictor 14 --nsink 1 --rate -1 | tee ../tests/output_fd60s_light_unbounded/main_1141-1.log

./main_light_source_unbounded --file ~/data/app/fd/credit-card.dat --nsource 2 --npredictor 4 --nsink 1 --rate -1 | tee ../tests/output_fd60s_light_unbounded/main_241-1.log
./main_light_source_unbounded --file ~/data/app/fd/credit-card.dat --nsource 2 --npredictor 8 --nsink 1 --rate -1 | tee ../tests/output_fd60s_light_unbounded/main_281-1.log
./main_light_source_unbounded --file ~/data/app/fd/credit-card.dat --nsource 2 --npredictor 13 --nsink 1 --rate -1 | tee ../tests/output_fd60s_light_unbounded/main_2131-1.log

./main_light_source_unbounded --file ~/data/app/fd/credit-card.dat --nsource 4 --npredictor 8 --nsink 1 --rate -1 | tee ../tests/output_fd60s_light_unbounded/main_481-1.log
./main_light_source_unbounded --file ~/data/app/fd/credit-card.dat --nsource 4 --npredictor 11 --nsink 1 --rate -1 | tee ../tests/output_fd60s_light_unbounded/main_4111-1.log
./main_light_source_unbounded --file ~/data/app/fd/credit-card.dat --nsource 5 --npredictor 10 --nsink 1 --rate -1 | tee ../tests/output_fd60s_light_unbounded/main_5101-1.log

#################################################################### BOUNDED BUFFER ####################################################################

# tests using the "heavy" source (max 16 PE - all physic cores)
./main_heavy_source_bounded --file ~/data/app/fd/credit-card.dat --nsource 1 --npredictor 1 --nsink 1 --rate -1 | tee ../tests/output_fd60s_heavy_bounded/main_111-1.log
./main_heavy_source_bounded --file ~/data/app/fd/credit-card.dat --nsource 1 --npredictor 2 --nsink 1 --rate -1 | tee ../tests/output_fd60s_heavy_bounded/main_121-1.log
./main_heavy_source_bounded --file ~/data/app/fd/credit-card.dat --nsource 1 --npredictor 4 --nsink 1 --rate -1 | tee ../tests/output_fd60s_heavy_bounded/main_141-1.log
./main_heavy_source_bounded --file ~/data/app/fd/credit-card.dat --nsource 1 --npredictor 8 --nsink 1 --rate -1 | tee ../tests/output_fd60s_heavy_bounded/main_181-1.log
./main_heavy_source_bounded --file ~/data/app/fd/credit-card.dat --nsource 1 --npredictor 14 --nsink 1 --rate -1 | tee ../tests/output_fd60s_heavy_bounded/main_1141-1.log

./main_heavy_source_bounded --file ~/data/app/fd/credit-card.dat --nsource 2 --npredictor 4 --nsink 1 --rate -1 | tee ../tests/output_fd60s_heavy_bounded/main_241-1.log
./main_heavy_source_bounded --file ~/data/app/fd/credit-card.dat --nsource 2 --npredictor 8 --nsink 1 --rate -1 | tee ../tests/output_fd60s_heavy_bounded/main_281-1.log
./main_heavy_source_bounded --file ~/data/app/fd/credit-card.dat --nsource 2 --npredictor 13 --nsink 1 --rate -1 | tee ../tests/output_fd60s_heavy_bounded/main_2131-1.log

./main_heavy_source_bounded --file ~/data/app/fd/credit-card.dat --nsource 4 --npredictor 8 --nsink 1 --rate -1 | tee ../tests/output_fd60s_heavy_bounded/main_481-1.log
./main_heavy_source_bounded --file ~/data/app/fd/credit-card.dat --nsource 4 --npredictor 11 --nsink 1 --rate -1 | tee ../tests/output_fd60s_heavy_bounded/main_4111-1.log
./main_heavy_source_bounded --file ~/data/app/fd/credit-card.dat --nsource 5 --npredictor 10 --nsink 1 --rate -1 | tee ../tests/output_fd60s_heavy_bounded/main_5101-1.log

# tests using the "light" source (max 16 PE - all physic cores)
./main_light_source_bounded --file ~/data/app/fd/credit-card.dat --nsource 1 --npredictor 1 --nsink 1 --rate -1 | tee ../tests/output_fd60s_light_bounded/main_111-1.log
./main_light_source_bounded --file ~/data/app/fd/credit-card.dat --nsource 1 --npredictor 2 --nsink 1 --rate -1 | tee ../tests/output_fd60s_light_bounded/main_121-1.log
./main_light_source_bounded --file ~/data/app/fd/credit-card.dat --nsource 1 --npredictor 4 --nsink 1 --rate -1 | tee ../tests/output_fd60s_light_bounded/main_141-1.log
./main_light_source_bounded --file ~/data/app/fd/credit-card.dat --nsource 1 --npredictor 8 --nsink 1 --rate -1 | tee ../tests/output_fd60s_light_bounded/main_181-1.log
./main_light_source_bounded --file ~/data/app/fd/credit-card.dat --nsource 1 --npredictor 14 --nsink 1 --rate -1 | tee ../tests/output_fd60s_light_bounded/main_1141-1.log

./main_light_source_bounded --file ~/data/app/fd/credit-card.dat --nsource 2 --npredictor 4 --nsink 1 --rate -1 | tee ../tests/output_fd60s_light_bounded/main_241-1.log
./main_light_source_bounded --file ~/data/app/fd/credit-card.dat --nsource 2 --npredictor 8 --nsink 1 --rate -1 | tee ../tests/output_fd60s_light_bounded/main_281-1.log
./main_light_source_bounded --file ~/data/app/fd/credit-card.dat --nsource 2 --npredictor 13 --nsink 1 --rate -1 | tee ../tests/output_fd60s_light_bounded/main_2131-1.log

./main_light_source_bounded --file ~/data/app/fd/credit-card.dat --nsource 4 --npredictor 8 --nsink 1 --rate -1 | tee ../tests/output_fd60s_light_bounded/main_481-1.log
./main_light_source_bounded --file ~/data/app/fd/credit-card.dat --nsource 4 --npredictor 11 --nsink 1 --rate -1 | tee ../tests/output_fd60s_light_bounded/main_4111-1.log
./main_light_source_bounded --file ~/data/app/fd/credit-card.dat --nsource 5 --npredictor 10 --nsink 1 --rate -1 | tee ../tests/output_fd60s_light_bounded/main_5101-1.log

