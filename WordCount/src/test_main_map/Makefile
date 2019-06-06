# Author: Alessandra Fais
# Date: 06/06/2019

FF_ROOT			= $(HOME)/fastflow/
WF_INCLUDES		= $(HOME)/WindFlow/includes/
INCLUDE_DIR		= ../../includes
OUT_DIR			= ../../bin

CXX 			= /usr/local/gcc-7.2.0/bin/g++
CXXFLAGS		= -std=c++17
INCLUDES		= -I $(FF_ROOT) -I $(WF_INCLUDES) -I $(INCLUDE_DIR) -I .
MACRO           = -DFF_BOUNDED_BUFFER
OPTFLAGS		= -g -O3 -finline-functions
LDFLAGS			= -pthread

SRCS			:= $(wildcard *.cpp)
TGTS			:= $(basename $(SRCS))

all: $(TGTS)

%: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(MACRO) $(OPTFLAGS) $(LDFLAGS) -o $(OUT_DIR)/$@ $<

clean:
	rm -f $(OUT_DIR)/*

.DEFAULT_GOAL := all
.PHONY: all clean