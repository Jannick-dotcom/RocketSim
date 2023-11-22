#include "mainGame.hpp"

using namespace std;
#ifdef logging
ofstream logfile(OutputPath + "log.csv", ios::out);

//create first line with elements of vals for csv file
string createHeader()
{
    string header = "time;spd;angle;g;accVehicle;vehThrust;ctEngines;vehMass;coefficient;density;pressure;accDrag;suicideBurnActive;entryBurnActive;throttle;throttleSet;alt\n";
    return header;
}

//create line with values of vals for csv file
string logValsToCsv(struct vals *temp)
{
    string line = 
    to_string(double(temp->timeS) + (temp->timeMS / 1000.0)) + ";" + 
    to_string(temp->speed.getlength()) + ";" + 
    to_string(temp->orientation.getx()) + ";" + 
    to_string(temp->g.getlength()) + ";" + 
    to_string(temp->accVehicle) + ";" + 
    to_string(temp->engThrust) + ";" + 
    to_string(temp->ctEngines) + ";" + 
    to_string(temp->vehMass) + ";" + 
    to_string(temp->coefficient) + ";" + 
    to_string(temp->density) + ";" + 
    to_string(temp->pressure) + ";" + 
    to_string(temp->drag.getlength()) + ";" + 
    to_string(temp->suicideBurnActive) + ";" + 
    to_string(temp->entryBurnActive) + ";" + 
    to_string(temp->throttle) + ";" + 
    to_string(temp->throttleSet) + ";" + 
    to_string(temp->alt) + "\n";
    return line;
}
#endif

void printRocket(struct vals *temp)
{
    cout << "  ^\n"
         << " / \\\n"
         << "/   \\\n"
         << "| S |\n"
         << "| P |\n" 
         << "| A |\n" 
         << "| C |\n" 
         << "| E |\n" 
         << "| J |\n" 
         << "| _ |\n" 
         << "| ^ |\n";

    if (temp->throttle > 0 && temp->alt > 0.5)
    {
        for (uint8_t i = 0; i < (int)(temp->throttle * 5); i++)
        {
            cout << " ***\n";
        }
    }
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
    double verticalSpeed = (temp->position.normalize() * temp->speed);
    std::cout << "Speed: " << temp->speed.getlength() << " m/s\n";
    std::cout << "Vertical Speed: " << verticalSpeed << " m/s\n";
    std::cout << "Altitude: " << temp->alt << " m\n";
    // std::cout << "Angle: " << temp->angle << " °\n";
    std::cout << "total Acceleration: " << ((temp->accVehicle * temp->throttle * temp->orientation.normalize()) + temp->drag + temp->g).getlength() << " m/s²\n";
    std::cout << "g: " << temp->g.getlength() << "\t Drag: " << temp->drag.getlength() << "\t Engines: " << (temp->accVehicle * temp->throttle) << "\n";
    std::cout << "Fuel: " << ((temp->vehMass - temp->dryMass) / (temp->initialMass- temp->dryMass)) * 100.0 << " %\n";
    std::cout << "Fuel for " << (temp->vehMass - temp->dryMass) / (temp->fuelConsumption * temp->ctEngines * temp->throttle) << " s\n";
    std::cout << "Throttle: " << temp->throttle * 100.0f << " %\n";
    std::cout << "Active Engines: " << (int)temp->ctEngines << "\n";
    std::cout << "\n";
    printRocket(temp);
    printGround(temp);
}


void init(struct vals *temp)
{
    #ifdef logging
    logfile.write(createHeader().c_str(), createHeader().length());
    #endif
    temp->timeS = 0;
    temp->timeMS = 0;
    // temp->gravConst = 6.6743 * pow(10, -11);
    temp->coefficient = 100;
    temp->alt = 0.0f;                      //[m]
    // temp->earthMass = 5.972 * pow(10, 24); //[kg]
    // temp->earthRadius = 6371000.0;
    temp->accVehicle = 0.0f;                                                                //[m/s²]
    temp->vehMass = temp->initialMass = 267000; //[kg]
    // temp->dryMass = 20000.0f;
    // temp->engThrust = 1000000.0f;
    temp->ctEngines = 9;
    temp->vehThrust = temp->engThrust * temp->ctEngines; //[N]
    temp->orientation = vektor(0.0f, 1.0f, 0.0f);

    temp->fuelConsumption = 161.0f; //[kg/s] per engine
    temp->exhaustVelocity = 4500.0f;  //[m/s]
    temp->throttle = 0.0f;                                     //[%]
    temp->throttleSet = 0.0f;                                 //[%]
    // temp->throttleResponse = 80.0f;                            //[%/s]

    // temp->stepsize = 0.001; //[s]

    temp->radius = 12.0;
    temp->area = M_1_PI * pow(temp->radius, 2);
    temp->density = 0;
    // temp->SeaLvlpressure = 1013.0;
    temp->suicideBurnActive = 0;
    temp->entryBurnActive = 0;

    temp->position = vektor(0.0, temp->earthRadius, 0.0);
}

void doStep(struct vals *temp)
{
    if (temp->throttleSet > 1.0)
        temp->throttleSet = 1.0;
    else if (temp->throttleSet < 0.0)
        temp->throttleSet = 0.0;

    if (temp->vehMass <= temp->dryMass)
        temp->ctEngines = 0;

    if(temp->ctEngines > 9)
        temp->ctEngines = 9;

    if(temp->alt > 120000)
    {
        temp->density = 0;
        temp->drag = vektor(0.0, 0.0, 0.0);
    } 
    else
    {
        if((temp->position.normalize() * temp->speed) > 0)
        {
            temp->coefficient = 50;
        }
        else
        {
            temp->coefficient = 82;
        }
        temp->pressure = temp->SeaLvlpressure * pow(1.0 - ((0.0065 * temp->alt) / (15.0+0.0065*temp->alt+273.15)),5.257); 
        temp->density = temp->pressure / (287.058 * 293.15); //Gaskonstante und 20°C
        double absdrag = (temp->coefficient * temp->density * pow(temp->speed.getlength(), 2) * temp->area * 0.5) / temp->vehMass;
        temp->drag = temp->speed.normalize() * -absdrag;
    }

    temp->engThrust = temp->fuelConsumption * temp->exhaustVelocity;
    temp->vehThrust = temp->engThrust * temp->ctEngines;
    temp->accVehicle = (temp->vehThrust / temp->vehMass);


    if(temp->alt < 0)
    {
        temp->position = temp->position.normalize() * temp->earthRadius;
        temp->speed = vektor(0.0, 0.0, 0.0);
    }
    else
    {
        double forceToEarth = temp->gravConst * ((temp->earthMass*temp->vehMass) / ((temp->earthRadius+temp->alt) * (temp->earthRadius+temp->alt)));
        double accelerationToEarth = forceToEarth / temp->vehMass;
        temp->g = (temp->position.normalize()) * -accelerationToEarth;
    }

    if(abs(temp->throttle - temp->throttleSet) > temp->throttleResponse * temp->stepsize / 100.0)
    {
        temp->throttle = temp->throttle + (temp->throttleResponse / 100.0 * temp->stepsize * ((temp->throttleSet - temp->throttle) >= 0 ? 1 : -1));
    }
    else
    {
        temp->throttle = temp->throttleSet;
    }

    vektor currentAcceleration = (temp->g + (temp->accVehicle * temp->throttle) * temp->orientation.normalize() + temp->drag);
    temp->speed = temp->speed + currentAcceleration * temp->stepsize;

    temp->position = temp->position + (temp->speed * temp->stepsize);
    temp->alt = temp->position.getlength() - temp->earthRadius;

    temp->vehMass = temp->vehMass - (temp->fuelConsumption * temp->throttle * temp->ctEngines * temp->stepsize);
    temp->timeMS = uint16_t(temp->timeMS + (1000.0 * temp->stepsize));
    if(temp->timeMS >= 1000)
    {
        temp->timeMS = uint16_t(temp->timeMS - uint16_t(1000U));
        temp->timeS++;
    }

    #ifdef logging
        logfile << logValsToCsv(temp);
    #endif
}

void lastStep()
{
#ifdef logging
    logfile.close();
#endif
}