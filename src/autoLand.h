#ifndef autoLand_h
#define autoLand_h

#include "mainGame.h"
#include "helpers.h"

void autoLand()
{
    tSuicide = -spdy / (aSuicideTarget + g);
    dSuicide = 0.5 * (aSuicideTarget + g) * pow(tSuicide, 2);

    if (alt < 100000 && spdy < -500 && entryBurnActive == 0)
    {
        throttle = 1;
        entryBurnActive = 1;
    }
    else if (spdy >= -500 && entryBurnActive == 1)
    {
        throttle = 0;
        entryBurnActive = 2;
    }
    else if ((dSuicide >= alt && alt < 100000) || suicideBurnActive)
    {
        if (spdy > -2.0)
            aSuicideTarget = -g;
        throttle = (aSuicideTarget) / accEngines;
        if (throttle > 1.0)
            throttle = 1.0;
        suicideBurnActive = 1;
    }
}


#endif