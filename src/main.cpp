#include <iostream>
#include <unistd.h>
#include <math.h>
unsigned int microsecond = 1000000;

using namespace std;

#include "mainGame.h"
#include "helpers.h"

void autoLand()
{
    tSuicide = -spdy / (aSuicideTarget + g);
    dSuicide = 0.5 * (aSuicideTarget + g) * pow(tSuicide, 2);

    if ((dSuicide >= alt && alt < 100000) || suicideBurnActive)
    {
        if (spdy > -5.0)
            aSuicideTarget = -g;
        throttle = (aSuicideTarget) / accEngines;
        if (throttle > 1.0)
            throttle = 1.0;
        suicideBurnActive = 1;
    }
}

int main()
{
    const uint64_t trys = 10000;
    uint64_t indexMin;

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

    //replay
    init();
    aSuicideTarget = (aSuicideTarget / 2.0) + (indexMin - (trys / 2.0f)) / (trys / 10.0f);
    while (alt > 0 && spdy <= 0)
    {
        //std::system("clear");

        cout << "Speed: " << spdy << " m/s" << endl;
        cout << "Altitude: " << alt << " m" << endl;
        cout << "Acceleration: " << (accEngines * throttle) << " m/s²" << endl;
        cout << "Acceleration Target: " << aSuicideTarget << " m/s²" << endl;
        cout << "tSuicide: " << tSuicide << " s" << endl;
        cout << "dSuicide: " << dSuicide << " m" << endl;
        cout << "Throttle: " << throttle * 100.0f << " %" << endl;
        cout << endl;

        // usleep((stepsize/2.0)*microsecond);
        doStep();
        autoLand();
    }
    return 0;
}