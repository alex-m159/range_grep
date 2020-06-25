#!/bin/bash
echo "Compiling range.cpp"
g++ -c range.cpp 
echo "Compiling test_range.cpp"
g++ -c test_range.cpp
echo "Linking"
g++ range.cpp test_range.cpp -O0 -pedantic -static -Wall --std=c++17 -lrt -lpthread -o test_range
echo "Running"
./test_range
