# Author: Alessandra Fais
# Date: 16/07/2019

all: test_main_chained test_main_unchained

test_main_chained:
	$(MAKE) -C test_main_chained

test_main_unchained:
	$(MAKE) -C test_main_unchained

clean:
	$(MAKE) clean -C test_main_chained
	$(MAKE) clean -C test_main_unchained

.DEFAULT_GOAL := all
.PHONY: all test_main_chained test_main_unchained clean