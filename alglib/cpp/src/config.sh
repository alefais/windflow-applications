# Script for alglib library: compile all and build .so library
# @author Alessandra Fais
# @date 16/07/2019

/usr/local/gcc-7.2.0/bin/g++ -Wall -g -O2 -shared -o libalglib.so -fPIC *.cpp
