# Author: Alessandra Fais
# Date: 17/07/2019

#FF_ROOT        = /Users/ale/fastflow                       # MACOS
FF_ROOT         = $(HOME)/WindFlow/fastflow
#WF_INCLUDES    = /Users/ale/WindFlow/WindFlow/includes     # MACOS
WF_INCLUDES     = $(HOME)/WindFlow/includes
ALGLIB_INCLUDES = ../../../alglib/cpp/src
INCLUDE_DIR		= ../../includes
OUT_DIR			= ../../bin

#CXX            = g++-9                                     # MACOS
CXX 			= /usr/local/gcc-7.2.0/bin/g++
CXXFLAGS		= -std=c++17
INCLUDES		= -I $(FF_ROOT) -I $(WF_INCLUDES) -I $(INCLUDE_DIR) -I . -I $(ALGLIB_INCLUDES)
MACRO           = -DFF_BOUNDED_BUFFER
OPTFLAGS		= -g -O3 -finline-functions
LDFLAGS			= -pthread -L ../../../alglib/cpp/src -lalglib

SRCS			:= $(wildcard *.cpp)
TGTS			:= $(basename $(SRCS))

all: $(TGTS)

%: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(MACRO) $(OPTFLAGS) $(LDFLAGS) -o $(OUT_DIR)/$@ $<

clean:
	rm -f $(OUT_DIR)/*

.DEFAULT_GOAL := all
.PHONY: all clean