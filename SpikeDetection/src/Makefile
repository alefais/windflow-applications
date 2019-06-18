# Author: Alessandra Fais
# Date: 16/05/2019

all: test_main_map test_main_keyfarm test_average_calculator

test_main_map:
	$(MAKE) -C test_main_map

test_main_keyfarm:
	$(MAKE) -C test_main_keyfarm

test_average_calculator:
	$(MAKE) -C test_average_calculator

clean:
	$(MAKE) clean -C test_main_map
	$(MAKE) clean -C test_average_calculator

.DEFAULT_GOAL := all
.PHONY: all test_main_map test_main_keyfarm test_average_calculator clean