# Author: Alessandra Fais
# Date: 04/06/2019

all: test_main_map

test_main_map:
	$(MAKE) -C test_main_map

test_main_keyfarm:
	$(MAKE) -C test_main_keyfarm

clean:
	$(MAKE) clean -C test_main_map

.DEFAULT_GOAL := all
.PHONY: all test_main_map clean