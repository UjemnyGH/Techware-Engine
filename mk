#!/bin/bash

g++ -m64 -Ofast -Os -Wall -Wextra -Wpedantic -std=c++2b -o test program.cpp -I engine/vendor/linux/include -lGL -lglfw -lm -lpthread