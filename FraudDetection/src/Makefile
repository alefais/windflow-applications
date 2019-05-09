# Author: Alessandra Fais
# Date: 07/05/2019

all: test_light_source test_heavy_source test_markov_model test_model_predictor

test_light_source:
	$(MAKE) -C test_light_source

test_heavy_source:
	$(MAKE) -C test_heavy_source

test_markov_model:
	$(MAKE) -C test_markov_model

test_model_predictor:
	$(MAKE) -C test_model_predictor

clean:
	$(MAKE) clean -C test_light_source
	$(MAKE) clean -C test_heavy_source
	$(MAKE) clean -C test_markov_model
	$(MAKE) clean -C test_markov_model

.DEFAULT_GOAL := all
.PHONY: all test_light_source test_heavy_source test_markov_model test_model_predictor clean