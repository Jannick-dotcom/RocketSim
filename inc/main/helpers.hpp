// #ifndef helpers_hpp
// #define helpers_hpp
#pragma once

#include <math.h>
#include <stdint.h>
#include "mainGame.hpp"
struct parameter
{
    int id;
    double valToVariate;
    // struct vals *values;
};
// using namespace std;

double findGAtAlt(struct vals *temp, double alt);

double rad2deg(double rad);

double deg2rad(double deg);

extern const std::string OutputPath;// = "/home/jannick/Desktop/outputs/";

// #endif