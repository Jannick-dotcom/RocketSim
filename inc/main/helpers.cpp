#include "helpers.hpp"

double findGAtAlt(struct vals *temp, double alt)
{
    double tempG = -temp->gravConst * (temp->earthMass / pow(alt + temp->earthRadius, 2));
    return tempG; //-temp->gravConst * (temp->earthMass / pow(temp->alt + temp->earthRadius, 2));
}

double rad2deg(double rad)
{
    return (rad * 360.0) / (2.0 * M_PI);
}

double deg2rad(double deg)
{
    return (deg / 360.0) * (2.0 * M_PI);
}