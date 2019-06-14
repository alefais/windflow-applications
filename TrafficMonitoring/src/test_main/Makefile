# Author: Alessandra Fais
# Date: 13/06/2019

FF_ROOT			= $(HOME)/fastflow/
WF_INCLUDES		= $(HOME)/WindFlow/includes/
GDAL_INCLUDE    = /usr/local/Cellar/gdal/2.4.1_1/include/
INCLUDE_DIR		= ../../includes
OUT_DIR			= ../../bin
GDAL_DIR        = /usr/local/Cellar/gdal/2.4.1_1/lib/

CXX 			= /usr/local/gcc-7.2.0/bin/g++
CXXFLAGS		= -std=c++17
INCLUDES		= -I $(FF_ROOT) -I $(WF_INCLUDES) -I $(INCLUDE_DIR) -I .
MACRO           = -DFF_BOUNDED_BUFFER
OPTFLAGS		= -g -O3 -finline-functions
LDFLAGS			= -pthread -L $(GDAL_DIR) -lgdal

SRCS			:= $(wildcard *.cpp)
TGTS			:= $(basename $(SRCS))

all: $(TGTS)

%: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(MACRO) $(OPTFLAGS) $(LDFLAGS) -o $(OUT_DIR)/$@ $<

clean:
	rm -f $(OUT_DIR)/*

.DEFAULT_GOAL := all
.PHONY: all clean