#ifndef mainGame_h
#define mainGame_h

#include <math.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include "vectors.hpp"

#define testing // comment this out to disable testing mode (no graphics)
#define multithreading
// #define asFastAsPossible // comment this out to disable asFastAsPossible mode (no sleep)

struct vals
{
    vektor speed;
    vektor position;
    vektor orientation;

    vektor g;           //[m/s²]
    double earthMass;   //[kg]
    double earthRadius; //[m]
    double gravConst;

    double accVehicle; //[m/s²]
    double vehThrust;  //[N]
    double engThrust;  //[N]
    int ctEngines;

    double initialMass;
    double vehMass;                 //[kg]
    double dryMass;                 //[kg]

    double radius;                  //[m] of rocket
    double area;                    //[m²]
    double coefficient; //????????
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