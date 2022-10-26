#!/bin/bash
g++ -Wall src/main.cpp inc/autoland/autoland.cpp inc/vectors/vectors.cpp inc/main/*.cpp -pthread -Isrc -Iinc/main -Iinc/autoland -Iinc/vectors -o build/a.out
./build/a.out