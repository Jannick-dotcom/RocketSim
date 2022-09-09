#pragma once

#include <stdint.h>
#include "vectors.hpp"

class Vehicle
{
public:
    vektor speed;
    vektor position;
    double height;
    double witdh;
    double dryMass;
    double fuelMass;
    uint16_t engineCT;
    double engThrust;       //[N]
    double fuelConsumption; //[kg/s]
    double throttle;
};