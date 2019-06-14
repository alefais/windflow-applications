# Author: Alessandra Fais
# Date: 13/06/2019

all: test_main test_gdal

test_main:
	$(MAKE) -C test_main

test_gdal:
	$(MAKE) -C test_gdal

clean:
	$(MAKE) clean -C test_main
	$(MAKE) clean -C test_gdal

.DEFAULT_GOAL := all
.PHONY: all test_main test_gdal clean