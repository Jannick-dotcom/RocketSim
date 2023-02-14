#ifndef mainGame_h
#define mainGame_h

#include <math.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include "vectors.hpp"
#include "helpers.hpp"

// #define testing // comment this out to disable testing mode (no graphics)
#define multithreading
// #define logging
// #define asFastAsPossible // comment this out to disable asFastAsPossible mode (no sleep)

struct vals
{
    uint64_t timeS;      //[s]
    uint16_t timeMS;     //[ms]
    vektor speed;
    vektor position;
    vektor orientation;

    vektor g;           //[m/s²]
    const double earthMass;   //[kg]
    const double earthRadius; //[m]
    const double gravConst; 

    double accVehicle; //[m/s²]
    double vehThrust;  //[N]
    double engThrust;  //[N]
    int ctEngines;

    double initialMass; //[kg]
    double vehMass;                 //[kg]
    const double dryMass;                 //[kg]

    double radius;                  //[m] of rocket
    double area;                    //[m²]
    double coefficient; //????????
    double density;
    const double SeaLvlpressure;
    double pressure;
    vektor drag;

    int suicideBurnActive;
    int entryBurnActive;
    double fuelConsumption; //[kg/s]
    double exhaustVelocity; //[m/s]
    double throttle;        //[%]
    double throttleSet;    //[%]
    const double throttleResponse;

    double alt;            //[m]
    const double stepsize;       //[s]
    vals() :
        earthMass(5.972 * pow(10, 24)), //[kg]
        earthRadius(6371000.0),         //[m]
        gravConst(6.6743 * pow(10, -11)),
        dryMass(20000.0f),    //[kg]
        SeaLvlpressure(1013.0), //[hPa]
        throttleResponse(80.0f), //[%/s]
        stepsize(0.001) //[s]
    {};
};

void printVals(struct vals *temp);
void init(struct vals *temp);
void doStep(struct vals *temp);
void lastStep();

#endif