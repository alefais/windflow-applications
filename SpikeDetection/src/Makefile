# Author: Alessandra Fais
# Date: 18/07/2019

all: test_main_chained test_main_unchained

test_main_chained:
	$(MAKE) -C test_main_chained

test_main_unchained:
	$(MAKE) -C test_main_unchained

test_average_calculator:
	$(MAKE) -C test_average_calculator

clean:
	$(MAKE) clean -C test_main_chained
	$(MAKE) clean -C test_main_unchained
	$(MAKE) clean -C test_average_calculator

.DEFAULT_GOAL := all
.PHONY: all test_main_chained test_main_unchained test_average_calculator clean