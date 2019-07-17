# Makefile for alglib library: compile all and build .a library
# @author Alessandra Fais
# @date   17/07/2019

CXX 	 = /usr/local/gcc-7.2.0/bin/g++
CXXFLAGS = -g -Wall -O2
LOBJS	 = alglibinternal.o alglibmisc.o ap.o dataanalysis.o diffequations.o fasttransforms.o integration.o interpolation.o linalg.o optimization.o solvers.o specialfunctions.o statistics.o

PROG 	 = lib
INCLUDES = -I .
LIBPATH	 = .

.SUFFIXES: .cpp .o

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCLUDES) $< -o $@

all: libalglib.a

lib: libalglib.a

libalglib.a: $(LOBJS)
	$(AR) -csru $@ $(LOBJS)

clean:
	rm -rf *.o
