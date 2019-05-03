FF_ROOT			= $(HOME)/fastflow/
OUT_DIR			= ./out
INCLUDE_DIR		= $(HOME)/WindFlow/includes/

CXX 			= /usr/local/gcc-7.2.0/bin/g++
CXXFLAGS		= -std=c++17
INCLUDES		= -I $(FF_ROOT) -I $(INCLUDE_DIR) -I .
OPTFLAGS		= -g -O3 -finline-functions
LDFLAGS			= -pthread

SRCS			:= $(wildcard *.cpp)
TGTS			:= $(basename $(SRCS))

all: $(TGTS)

%: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OPTFLAGS) $(LDFLAGS) -o $(OUT_DIR)/$@ $<

clean:
	rm -f $(OUT_DIR)/*

.DEFAULT_GOAL := all
.PHONY: all clean

# /usr/local/gcc-7.2.0/bin/g++ -std=c++17 -I $HOME/fastflow -I $HOME/WindFlow/includes -I . -O3 -pthread -o main main.cpp