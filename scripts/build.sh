#!/bin/bash
yes | sudo apt-get install libboost-all-dev
mkdir build
rm build/a.out
g++ -Wall src/main.cpp inc/autoland/autoland.cpp inc/vectors/vectors.cpp inc/main/*.cpp -pthread -Isrc -Iinc/main -Iinc/autoland -Iinc/vectors -o build/a.out