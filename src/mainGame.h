#ifndef mainGame_h
#define mainGame_h

#include <math.h>
#include <stdint.h>
#include <iostream>
#include "vectors/vectors.hpp"

using namespace std;

struct vals
{
    double spdy; //[m/s]
    double spdx; //[m/s]
    //vektor speed;
    double angle; //[°]

    double g;           //[m/s²]
    double earthMass;   //[kg]
    double earthRadius; //[m]
    const double gravConst = 6.674 * pow(10, -11);

    double accVehicle; //[m/s²]
    double vehThrust;  //[N]
    double engThrust;  //[N]
    int ctEngines;

    const double initialMass = 267000;
    double vehMass;                 //[kg]
    double dryMass;                 //[kg]

    double radius;                  //[m]
    double area;                    //[m²]
    const double coefficient = 100; //????????
    double density;
    double SeaLvlpressure;
    double pressure;
    double accDragX;
    double accDragY;

    double aSuicideTarget = 0;
    double fuelConsumption; //[kg/s]
    double throttle;        //[%]

    double alt;            //[m]
    double stepsize;       //[s]
};

void printRocket(struct vals *temp)
{
    cout << "/ \\\n"
         //<< "/  \\" << "\n"
         << "|S|\n"
         << "|P|\n" 
         << "|A|\n" 
         << "|C|\n" 
         << "|E|\n" 
         << "|J|\n" 
         << "|_|\n" 
         << "|^|\n";

    if (temp->throttle > 0 && temp->alt > 0.5)
        cout << " *\n";
}

void printGround(struct vals *temp)
{
    if (temp->alt < 50)
    {
        for (uint8_t i = 0; i < temp->alt / 2; i++)
        {
            cout << "\n";
        }
        for (uint8_t i = 0; i < 50; i++)
        {
            cout << "_";
        }
        cout << "\n";
    }
}

void printVals(struct vals *temp)
{
    std::cout << "Speed: " << temp->spdy << " m/s\n";
    std::cout << "Altitude: " << temp->alt << " m\n";
    std::cout << "Angle: " << temp->angle << " °\n";
    std::cout << "total Acceleration: " << (temp->accVehicle * temp->throttle) + temp->accDragY + temp->g << " m/s²\n";
    std::cout << "g: " << temp->g << "\t Drag: " << temp->accDragY << "\t Engines: " << (temp->accVehicle * temp->throttle) << "\n";
    std::cout << "Suicide Acceleration Error: " << (temp->accVehicle * temp->throttle) + temp->accDragY + temp->g - temp->aSuicideTarget << "m/s²\n";
    std::cout << "Fuel: " << ((temp->vehMass - temp->dryMass) / temp->initialMass) * 100.0 << " %\n";
    std::cout << "Throttle: " << temp->throttle * 100.0f << " %\n";
    std::cout << "Active Engines: " << temp->ctEngines << "\n";
    std::cout << "\n";
    printRocket(temp);
    printGround(temp);
}

void init(struct vals *temp)
{
    temp->spdy = 0.0f;                     //[m/s]
    temp->alt = 0.0f;                      //[m]
    temp->earthMass = 5.972 * pow(10, 24); //[kg]
    temp->earthRadius = 6371000.0;
    temp->g = -temp->gravConst * (temp->earthMass / pow(temp->alt + temp->earthRadius, 2)); //[m/s²]
    temp->accVehicle = 0.0f;                                                                //[m/s²]
    temp->vehMass = temp->initialMass;                                                              //[kg]
    temp->dryMass = 20000.0f;
    temp->engThrust = 1000000.0f;
    temp->ctEngines = 9;
    temp->vehThrust = temp->engThrust * temp->ctEngines; //[N]
    temp->angle = 0;

    temp->fuelConsumption = 1451.0f / temp->ctEngines; //[kg/s]
    temp->throttle = 0.0f;                                     //[%]

    temp->stepsize = 0.001; //[s]

    temp->radius = 12.0;
    temp->area = M_1_PI * pow(temp->radius, 2);
    temp->density = 0;
    temp->SeaLvlpressure = 1013.0;
    temp->accDragX = 0;
    temp->accDragY = 0;
}

void doStep(struct vals *temp)
{
    if (temp->throttle > 1.0)
        temp->throttle = 1.0;
    else if (temp->throttle < 0.0)
        temp->throttle = 0.0;

    if (temp->vehMass <= temp->dryMass)
        temp->ctEngines = 0;

    if(temp->ctEngines > 9)
        temp->ctEngines = 9;

    temp->pressure = temp->SeaLvlpressure * pow(1.0 - ((0.0065 * temp->alt) / (15.0+0.0065*temp->alt+273.15)),5.257); 

    temp->density = temp->pressure / (287.058 * 293.15); //Gaskonstante und 20°C

    temp->accDragY = (temp->coefficient * temp->density * pow(temp->spdy, 2) * temp->area * 0.5) / temp->vehMass;
    temp->accDragX = (temp->coefficient * temp->density * pow(temp->spdx, 2) * temp->area * 0.5) / temp->vehMass;
    if (temp->spdy > 0)
        temp->accDragY *= -1.0;
    if (temp->spdx > 0)
        temp->accDragX *= -1.0;

    temp->vehThrust = temp->engThrust * temp->ctEngines;
    temp->accVehicle = (temp->vehThrust / temp->vehMass);
    temp->spdy = temp->spdy + (temp->g + (temp->accVehicle * temp->throttle) + temp->accDragY) * temp->stepsize;
    temp->spdx = temp->spdx + (temp->g + (temp->accVehicle * temp->throttle) + temp->accDragX) * temp->stepsize;

    temp->alt = temp->alt + (temp->spdy * temp->stepsize);
    if(temp->ctEngines > 0)
        temp->fuelConsumption = 1451.0f / temp->ctEngines;
    else
        temp->fuelConsumption = 0;
    temp->vehMass = temp->vehMass - (temp->fuelConsumption * temp->throttle * temp->ctEngines * temp->stepsize);
    temp->g = -temp->gravConst * (temp->earthMass / pow(temp->alt + temp->earthRadius, 2));
}

#endif