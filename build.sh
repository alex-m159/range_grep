#!/bin/bash
g++ -c range.cpp 
g++ -c main.cpp
g++ range.o main.o -O3 -pedantic -static -Wall --std=c++17 -lrt -lpthread -o range

