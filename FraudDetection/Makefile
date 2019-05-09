# Author: Alessandra Fais
# Date: 07/05/2019

all:
	$(MAKE) -C src

test_light_source:
	$(MAKE) test_light_source -C src

test_heavy_source:
	$(MAKE) test_heavy_source -C src

test_markov_model:
	$(MAKE) test_markov_model -C src

test_model_predictor:
	$(MAKE) test_model_predictor -C src

clean:
	$(MAKE) clean -C src

.DEFAULT_GOAL := all
.PHONY: all test_light_source test_heavy_source test_markov_model test_model_predictor clean
