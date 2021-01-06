#ifndef autoLand_h
#define autoLand_h

#include "mainGame.h"
#include "helpers.h"

void autoLand(uint8_t print)
{
    tSuicide = -spdy / (aSuicideTarget + findMittelWertG(alt));
    dSuicide = 0.5 * (aSuicideTarget + findMittelWertG(alt)) * pow(tSuicide, 2);

    if (alt < 100000 && spdy < -500 && entryBurnActive == 0)
    {
        throttle = 1;
        entryBurnActive = 1;
        if (print)
        {
            cout << "Entry start:" << endl;
            printVals();
        }
    }
    else if (spdy >= -500 && entryBurnActive == 1)
    {
        throttle = 0;
        entryBurnActive = 2;
        aSuicideTarget = ((engThrust * ctEngines) / vehMass) + aSuicideTargetMod;
        if (print)
        {
            cout << "Entry finished:" << endl;
            printVals();
        }
        //ctEngines = 1;
        //printVals();
    }
    else if (((dSuicide >= alt && entryBurnActive == 2) || suicideBurnActive) && alt < 100000 && alt > 0)
    {
        if (spdy > -5.0)
        {
            aSuicideTarget = -g;
            ctEngines = 1;
        }
        ctEngines = int((aSuicideTarget * vehMass) / (engThrust)) + 1;
        throttle = (aSuicideTarget) / accVehicle;
        if (print && suicideBurnActive == 0)
        {
            cout << "Suicide start:" << endl;
            printVals();
        }
        suicideBurnActive = 1;
    }
    else if (alt < 0)
    {
        suicideBurnActive = 0;
        throttle = 0;
        if (print)
        {
            cout << "Suicide finished:" << endl;
            printVals();
        }
    }
}

#endif