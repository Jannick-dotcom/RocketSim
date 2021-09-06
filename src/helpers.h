#ifndef helpers_h
#define helpers_h

#include <math.h>
#include <stdint.h>

using namespace std;

double findGAtAlt(struct vals *temp, double alt)
{
    double tempG = -temp->gravConst * (temp->earthMass / pow(alt + temp->earthRadius, 2));
    return tempG; //-temp->gravConst * (temp->earthMass / pow(temp->alt + temp->earthRadius, 2));
}

#endif