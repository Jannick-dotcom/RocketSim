#!/bin/bash
yes | sudo apt-get install libboost-all-dev
rm -r build
mkdir build
g++ -Wall src/main.cpp inc/autoland/autoland.cpp inc/vectors/vectors.cpp inc/main/*.cpp -pthread -Isrc -Iinc/main -Iinc/autoland -Iinc/vectors -o build/a.out