#include <iostream>
#include <unistd.h>
#include <math.h>
unsigned int microsecond = 1000000;

using namespace std;

float spdy;            //[m/s]
float g;               //[m/s²]
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

void init()
{
    spdy = -500.0f;                      //[m/s]
    g = -9.81f;                          //[m/s²]
    accEngines = 0.0f;                   //[m/s²]
    vehMass = 100000.0f;                 //[kg]
    vehThrust = vehMass * abs(g) * 5.0f; //[N]

    fuelConsumption = 500.0f; //[kg/s]
    throttle = 0.0f;          //[%]

    alt = 11000.0f;   //[m]
    stepsize = 0.01f; //[s]
    suicideBurnActive = 0;
    aSuicideTarget = vehThrust / vehMass;
}

void doStep()
{
    //usleep((stepsize)*microsecond);
    //std::system("clear");
    accEngines = vehThrust / vehMass;
    spdy = spdy + (g + (accEngines * throttle)) * stepsize;
    alt = alt + (spdy * stepsize);
    vehMass = vehMass - ((fuelConsumption * throttle) * stepsize);
}

void autoLand()
{
    tSuicide = -spdy / (aSuicideTarget + g);
    dSuicide = 0.5 * (aSuicideTarget + g) * pow(tSuicide, 2);

    if (dSuicide >= alt || suicideBurnActive)
    {
        throttle = aSuicideTarget / accEngines;
        if (throttle > 1.0)
            throttle = 1.0;
        if (spdy > -2.0)
            aSuicideTarget = 9.81;
        suicideBurnActive = 1;
    }
}

float Jmin(float *arr1, float *arr2, uint16_t size, uint16_t *index)
{
    float currMin;
    for (auto ct = 0; ct < size; ct++)
    {
        if (ct == 0 || abs(currMin) > (abs(arr1[ct]) + abs(arr2[ct])) / 2.0f)
        {
            currMin = (abs(arr1[ct]) + abs(arr2[ct]) / 2.0f);
            *index = ct;
        }
    }
    return currMin;
}

int main()
{
    const uint16_t trys = 1000;
    uint16_t indexMin;

    float speeds[trys];
    float alts[trys];
    float accs[trys];
    for (auto i = 0; i < trys; i++)
    {
        init();
        aSuicideTarget = (aSuicideTarget / 2.0) + (i - (trys / 2.0f)) / (trys / 10.0f);
        while (alt > 0 && spdy < 0)
        {
            doStep();
            autoLand();
        }
        speeds[i] = spdy;
        alts[i] = alt;
        accs[i] = accEngines * throttle;
    }
    Jmin(speeds, alts, trys, &indexMin);
    init();
    aSuicideTarget = (aSuicideTarget / 2.0) + (indexMin - (trys / 2.0f)) / (trys / 10.0f);
    while (alt > 0 && spdy < 0)
    {
        doStep();
        autoLand();
        //std::system("clear");
        if (accEngines * throttle > 10.0)
        {
            cout << "Speed: " << spdy << " m/s" << endl;
            cout << "Altitude: " << alt << " m" << endl;
            cout << "Acceleration: " << accEngines * throttle << " m/s²" << endl;
            cout << "tSuicide: " << tSuicide << " s" << endl;
            cout << "dSuicide: " << dSuicide << " m" << endl;
            cout << "Throttle: " << throttle * 100.0f << " %" << endl << endl;
        }
        // usleep((stepsize/2.0)*microsecond);
    }
    return 0;
}