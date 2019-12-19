# Author: Alessandra Fais
# Date: 17/08/2019

all: test_main_chained test_main_unchained

test_main_chained:
	$(MAKE) -C test_main_chained

test_main_unchained:
	$(MAKE) -C test_main_unchained

test_gdal:
	$(MAKE) -C test_gdal

clean:
	$(MAKE) clean -C test_main_chained
	$(MAKE) clean -C test_main_unchained
	$(MAKE) clean -C test_gdal

.DEFAULT_GOAL := all
.PHONY: all test_main_chained test_main_unchained test_gdal clean