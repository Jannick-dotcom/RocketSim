#!/bin/bash
g++ -Wall src/main.cpp -pthread -Isrc/autoland -o build/a.out
./build/a.out