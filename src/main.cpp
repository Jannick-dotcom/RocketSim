#include <iostream>
#include <unistd.h>
#include <math.h>
unsigned int microsecond = 1000000;

using namespace std;

#include "mainGame.h"
#include "helpers.h"
#include "autoLand.h"

void executeFlightPath(uint8_t print)
{
    throttle = 1;
    if (print)
    {
        cout << "Launch:" << endl;
        printVals();
    }
    while (spdy < 1500)
    {
        doStep();
    }
    throttle = 0;
    if (print)
    {
        cout << "MECO:" << endl;
        printVals();
    }
    while (alt > 0)
    {
        doStep();
        autoLand(print);
    }
}

int main()
{
    const uint64_t trys = 1000000;
    uint64_t indexMin;

    float speeds[trys];
    float alts[trys];
    float accs[trys];
    for (auto i = 0; i < trys; i++)
    {
        init();
        aSuicideTargetMod = float(i - (trys / 2.0f)) / float(trys / 10.0f);
        executeFlightPath(0);
        speeds[i] = spdy;
        alts[i] = alt;
        accs[i] = accVehicle * throttle;
    }
    Jmin(speeds, alts, trys, &indexMin);

    //replay
    cout << "Replay of best Flight: [" << indexMin << "]" << endl;
    init();
    aSuicideTarget = (indexMin - (trys / 2.0f)) / (trys / 10.0f);
    executeFlightPath(1);

    cout << "End Result:" << endl;
    printVals();
    return 0;
}