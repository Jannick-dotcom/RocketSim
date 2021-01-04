#include <iostream>
#include <unistd.h>
#include <math.h>
unsigned int microsecond = 1000000;

using namespace std;

float spd = -500;     //[m/s]
float g = -9.81;       //[m/s²]
float accEngines = 0;  //[m/s²]
float accEnginesMax = 30; //[m/s²]
float alt = 11000;    //[m]
float stepsize = 0.01; //[s]

void doStep()
{
    usleep(stepsize * microsecond); 
    spd = spd + (g + accEngines) * stepsize;
    alt = alt + (spd * stepsize);
    accEnginesMax = accEnginesMax + (0.1 * accEngines);
}

void autoLand()
{
    static float tSurface;
    static float tSuicide;
    static float dSuicide;
    if(alt < 10000)
    {
        tSuicide = -spd / (accEnginesMax -g);
        dSuicide = 0.5 * (accEnginesMax - g) * pow(tSuicide,2);

        cout << "tSuicide = " << tSuicide << "s , dSuicide = " << dSuicide << endl;
        if(dSuicide >= alt)
        {
            accEngines = accEnginesMax;
        }
    }
}

int main()
{
    while(alt > 0)
    {
        doStep();
        autoLand();
        cout << spd << "m/s, " << alt << "m, " << accEnginesMax << "m/s²" << endl;
    }
}