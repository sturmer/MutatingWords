#!/bin/bash
clang++ -g -std=c++11 -Wall -Werror util.cc t.cc -lgtest -lpthread
