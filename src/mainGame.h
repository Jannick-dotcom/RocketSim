#ifndef mainGame_h
#define mainGame_h

#include <math.h>
#include <stdint.h>
#include <iostream>

float spdy; //[m/s]
float spdx; //[m/s]

float g;         //[m/s²]
float earthMass; //[kg]
float earthRadius;//[m]
const float gravConst = 6.674 * pow(10, -11);

float accVehicle;      //[m/s²]
float vehThrust;       //[N]
float engThrust;       //[N]
float ctEngines;
float vehMass;         //[kg]
float dryMass;         //[kg]

float fuelConsumption; //[kg/s]
float throttle;        //[%]

float alt;            //[m]
float stepsize;       //[s]
float aSuicideTarget; //[m/s]
float aSuicideTargetMod;
float tSurface;       //[s]
float tSuicide;       //[s]
float dSuicide;       //[s]
uint8_t suicideBurnActive;
uint8_t entryBurnActive;

void printVals()
{
    std::cout << "Speed: " << spdy << " m/s" << std::endl;
    std::cout << "Altitude: " << alt << " m" << std::endl;
    std::cout << "Acceleration: " << (accVehicle * throttle) << " m/s²" << std::endl;
    std::cout << "Acceleration Suicide Target: " << aSuicideTarget << " m/s²" << std::endl;
    std::cout << "Prop Mass: " << vehMass - dryMass << " kg" << std::endl;
    std::cout << "TWR: " << vehThrust / vehMass << std::endl;
    std::cout << "tSuicide: " << tSuicide << " s" << std::endl;
    std::cout << "dSuicide: " << dSuicide << " m" << std::endl;
    std::cout << "Throttle: " << throttle * 100.0f << " %" << std::endl;
    std::cout << "Active Engines: " << ctEngines << std::endl;
    std::cout << std::endl;
}

void init()
{
    spdy = 0.0f;                     //[m/s]
    alt = 0.0f;                 //[m]
    earthMass = 5.972 * pow(10, 24); //[kg]
    earthRadius = 6371000.0;
    //gravConst = 6.6720 * pow(10, -11);
    g = -gravConst * (earthMass / pow(alt + earthRadius, 2)); //[m/s²]
    accVehicle = 0.0f;                                        //[m/s²]
    vehMass = 300000.0f;                                      //[kg]
    dryMass = 33000.0f;
    engThrust = 550000.0f;
    ctEngines = 9.0f;
    vehThrust = engThrust * ctEngines;                     //[N]

    fuelConsumption = 1451.0f; //[kg/s]
    throttle = 0.0f;           //[%]

    stepsize = 0.0001f; //[s]
    suicideBurnActive = 0;
    entryBurnActive = 0;
    aSuicideTarget = vehThrust / vehMass;
    aSuicideTargetMod = 0;
}

void doStep()
{
    if (throttle > 1.0) throttle = 1.0;
    else if(throttle < 0.0) throttle = 0.0;

    if(vehMass < dryMass) ctEngines = 0;
    vehThrust = engThrust * ctEngines;
    accVehicle = vehThrust / vehMass;
    spdy = spdy + (g + (accVehicle * throttle)) * stepsize;
    alt = alt + (spdy * stepsize);
    vehMass = vehMass - ((fuelConsumption * throttle) * stepsize);
    g = -gravConst * (earthMass / pow(alt + earthRadius, 2));
}

#endif