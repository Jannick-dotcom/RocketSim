#include <iostream>
#include <unistd.h>
#include <math.h>
unsigned int microsecond = 1000000;

using namespace std;

#include "mainGame.h"
#include "helpers.h"


int main()
{
    const uint64_t trys = 1000;
    uint64_t indexMin;

    float speeds[trys];
    float alts[trys];
    float accs[trys];
    for (auto i = 0; i < trys; i++)
    {
        init();
        aSuicideTarget = (aSuicideTarget / 2.0f) + float(i - (trys / 2.0f)) / float(trys / 10.0f);
        executeFlightPath();
        speeds[i] = spdy;
        alts[i] = alt;
        accs[i] = accEngines * throttle;
    }
    Jmin(speeds, alts, trys, &indexMin);

    //replay
    init();
    aSuicideTarget = (aSuicideTarget / 2.0) + (indexMin - (trys / 2.0f)) / (trys / 10.0f);
    executeFlightPath();

    cout << "Speed: " << spdy << " m/s" << endl;
    cout << "Altitude: " << alt << " m" << endl;
    cout << "Acceleration: " << (accEngines * throttle) << " m/s²" << endl;
    cout << "Acceleration Target: " << aSuicideTarget << " m/s²" << endl;
    cout << "tSuicide: " << tSuicide << " s" << endl;
    cout << "dSuicide: " << dSuicide << " m" << endl;
    cout << "Throttle: " << throttle * 100.0f << " %" << endl;
    cout << endl;

    cout << "[ " << indexMin << " ]" << endl;
    return 0;
}