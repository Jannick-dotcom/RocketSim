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
    return temp_tSuicide * temp_tSuicide * temp_netAcceleration * 0.5; // S = 1/2 * a * t²
}

double playBackwards(struct vals *values)
{
    struct vals temp;
    init(&temp);
    temp.dryMass = values->dryMass;
    temp.vehMass = values->vehMass;
    temp.ctEngines = 1;
    temp.stepsize = 0.001;
    temp.throttleSet = 1;
    while (temp.speed.getlength() < values->speed.getlength() && temp.vehMass > temp.dryMass)
    {
        doStep(&temp);
    }
    return temp.alt;
}

void autoland(struct vals *values)
{
    double verticalSpeed = values->speed * values->position.normalize();
    if(verticalSpeed > 0 && !values->suicideBurnActive)
    {
        return;
    }
    //Entry Burn
    double dSuicide = 0.0;
    values->orientation = values->speed.normalize() * -1; //Retrograde
    if(values->entryBurnActive == 0 && values->alt < 65e3 && values->speed.getlength() > 2300)
    {
        values->throttleSet = 1.0;
        values->entryBurnActive = 1;
    }
    else if(values->entryBurnActive == 1 && values->alt < 6e4 && values->speed.getlength() < 800)
    {
        values->throttleSet = 0.0;
        values->entryBurnActive = 2;
    }

    //Suicide Burn
    if(values->suicideBurnActive == 0)
    {
        for (uint8_t i = 9; i > 0; i--) //check all engine configurations
        {
            dSuicide = calculate_d_suicide(values, i);
            if(dSuicide > 900 && dSuicide <= 2000)
            {
                values->ctEngines = i;
                break;
            }
            else if(dSuicide > 2000 && i > 3)
            {
                values->ctEngines = i;
                break;
            }
            else if(dSuicide < 900 && i == 1)
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
    if ((dSuicide >= values->alt && values->alt > 0 && values->entryBurnActive == 2) || values->suicideBurnActive)
    {
        values->throttleSet = dSuicide / values->alt;
        if(values->throttleSet> 1.5 && values->ctEngines < 9)
        {
            values->ctEngines++;
        }
        else if(values->throttleSet<= 0.4 && values->ctEngines > 1)
        {
            values->ctEngines--;
        }
        values->suicideBurnActive = 1;
        values->entryBurnActive = 2;
    }
}