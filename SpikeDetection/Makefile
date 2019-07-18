# Author: Alessandra Fais
# Date: 18/07/2019

all:
	$(MAKE) -C src

test_main_chained:
	$(MAKE) test_main_chained -C src

test_main_unchained:
	$(MAKE) test_main_unchained -C src

test_average_calculator:
	$(MAKE) test_average_calculator -C src

clean:
	$(MAKE) clean -C src

.DEFAULT_GOAL := all
.PHONY: all test_main_chained test_main_unchained test_average_calculator clean
