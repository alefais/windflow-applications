# Author: Alessandra Fais
# Date: 17/08/2019

all:
	$(MAKE) -C src

test_main_chained:
	$(MAKE) test_main_chained -C src

test_main_unchained:
	$(MAKE) test_main_unchained -C src

test_gdal:
	$(MAKE) test_gdal -C src

clean:
	$(MAKE) clean -C src

.DEFAULT_GOAL := all
.PHONY: all test_main_chained test_main_unchained test_gdal clean
