# Author: Alessandra Fais
# Date: 09/07/2019

all:
	$(MAKE) -C src

test_light_source_chained:
	$(MAKE) test_light_source_chained -C src

test_light_source_unchained:
	$(MAKE) test_light_source_unchained -C src

test_heavy_source:
	$(MAKE) test_heavy_source -C src

test_markov_model:
	$(MAKE) test_markov_model -C src

test_model_predictor:
	$(MAKE) test_model_predictor -C src

clean:
	$(MAKE) clean -C src

.DEFAULT_GOAL := all
.PHONY: all test_light_source_chained test_light_source_unchained test_heavy_source test_markov_model test_model_predictor clean
