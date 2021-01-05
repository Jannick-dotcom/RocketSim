#ifndef mainGame_h
#define mainGame_h

#include <math.h>
#include <stdint.h>

float spdy; //[m/s]
float spdx; //[m/s]

float g;         //[m/s²]
float earthMass; //[kg]
float earthRadius;
const float gravConst = 6.674 * pow(10, -11);

float accEngines;      //[m/s²]
float vehThrust;       //[N]
float vehMass;         //[kg]
float fuelConsumption; //[kg/s]
float throttle;        //[%]

float alt;            //[m]
float stepsize;       //[s]
float aSuicideTarget; //[m/s]
float tSurface;
float tSuicide;
float dSuicide;
uint8_t suicideBurnActive;
uint8_t entryBurnActive;

void init()
{
    spdy = -1.0f;                     //[m/s]
    alt = 130000.0f;                 //[m]
    earthMass = 5.972 * pow(10, 24); //[kg]
    earthRadius = 6371000.0;
    //gravConst = 6.6720 * pow(10, -11);
    g = -gravConst * (earthMass / pow(alt + earthRadius, 2)); //[m/s²]
    accEngines = 0.0f;                                        //[m/s²]
    vehMass = 100000.0f;                                      //[kg]
    vehThrust = vehMass * fabs(g) * 6.0f;                     //[N]

    fuelConsumption = 5000.0f; //[kg/s]
    throttle = 0.0f;           //[%]

    stepsize = 0.1f; //[s]
    suicideBurnActive = 0;
    entryBurnActive = 0;
    aSuicideTarget = vehThrust / vehMass;
}

void doStep()
{
    accEngines = vehThrust / vehMass;
    spdy = spdy + (g + (accEngines * throttle)) * stepsize;
    alt = alt + (spdy * stepsize);
    vehMass = vehMass - ((fuelConsumption * throttle) * stepsize);
    g = -gravConst * (earthMass / pow(alt + earthRadius, 2));
}

#endif