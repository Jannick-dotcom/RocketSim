#include "autoland.hpp"

double calculate_d_suicide(struct vals *values, uint8_t ct_engines)
{
    if(ct_engines == 0)
    {
        ct_engines = values->ctEngines;
    }
    double temp_accVehicle = (values->engThrust * ct_engines) / values->vehMass;
    double temp_netAcceleration = temp_accVehicle - values->g.getlength(); //Max Possible Acceleration
    double temp_tSuicide = values->speed.getlength() / temp_netAcceleration;
    return (temp_tSuicide * temp_tSuicide * temp_netAcceleration) / 2; // S = 1/2 * a * t²
}

void autoland(struct vals *values)
{
    double dSuicide = 0.0;
    if(values->suicideBurnActive == 0)
    {
        for (uint8_t i = 9; i > 0; i--) //check all engine configurations
        {
            dSuicide = calculate_d_suicide(values, i);

            if(dSuicide > 900 && dSuicide < 5000)
            {
                values->ctEngines = i;
                break;
            }
        }
    }
    else
    {
        dSuicide = calculate_d_suicide(values, values->ctEngines);
    }
    double angleRetrograde = rad2deg(atan(values->speed.getx() / values->speed.gety()));
    values->angle = angleRetrograde;
    if(values->entryBurnActive == 0 && values->alt < 6e4 && values->speed.getlength() > 1000)
    {
        values->throttle = 1.0;
        values->entryBurnActive = 1;
    }
    else if(values->entryBurnActive == 1 && values->alt < 6e4 && values->speed.getlength() < 900)
    {
        values->throttle = 0.0;
        values->entryBurnActive = 2;
    }

    if ((dSuicide >= values->alt && values->alt > 0 && values->entryBurnActive == 2) || values->suicideBurnActive)
    {
        values->stepsize = 0.0001;
        values->throttle = dSuicide / values->alt;
        if(values->throttle > 1 && values->ctEngines < 9)
        {
            // values->ctEngines++;
        }
        else if(values->throttle <= 0.2 && values->ctEngines > 1)
        {
            // values->ctEngines--;
        }
        values->suicideBurnActive = 1;
    }
}