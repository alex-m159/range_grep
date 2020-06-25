#!/bin/bash
echo "Compiling"
g++ test_range.cpp -O0 -pedantic -static -Wall --std=c++17 -lrt -lpthread -o test_range
echo "Running"
./test_range
