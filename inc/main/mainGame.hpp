#ifndef mainGame_h
#define mainGame_h

#include <math.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include "vectors.hpp"

// #define testing // comment this out to disable testing mode (no graphics)
// #define asFastAsPossible // comment this out to disable asFastAsPossible mode (no sleep)

struct vals
{
    vektor speed;
    vektor position;
    double angle; //[°]

    vektor g;           //[m/s²]
    double earthMass;   //[kg]
    double earthRadius; //[m]
    const double gravConst = 6.6743 * pow(10, -11);

    double accVehicle; //[m/s²]
    double vehThrust;  //[N]
    double engThrust;  //[N]
    int ctEngines;

    const double initialMass = 267000;
    double vehMass;                 //[kg]
    double dryMass;                 //[kg]

    double radius;                  //[m] of rocket
    double area;                    //[m²]
    const double coefficient = 100; //????????
    double density;
    double SeaLvlpressure;
    double pressure;
    vektor drag;

    int suicideBurnActive;
    int entryBurnActive;
    double fuelConsumption; //[kg/s]
    double throttle;        //[%]

    double alt;            //[m]
    double stepsize;       //[s]
};

void printVals(struct vals *temp);
void init(struct vals *temp);
void doStep(struct vals *temp);

#endif