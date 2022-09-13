#!/bin/bash
g++ -Wall src/main.cpp -pthread -Isrc -o build/a.out
./build/a.out