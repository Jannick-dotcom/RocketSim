#ifndef helpers_h
#define helpers_h

#include <math.h>
#include <stdint.h>

using namespace std;

float findMittelWertG(float altitude)
{
    float oben = (gravConst * earthMass) / (altitude + earthRadius);
    float unten = (gravConst * earthMass) / (earthRadius);
    return (oben - unten) / altitude;
}

float Jmin(float *arr1, float *arr2, uint64_t size, uint64_t *index)
{
    float currMin;
    for (auto ct = 0; ct < size; ct++)
    {
        if (ct == 0 || fabs(currMin) > (fabs(arr1[ct]) + fabs(arr2[ct])) / 2.0f)
        {
            currMin = (fabs(arr1[ct]) + fabs(arr2[ct]) / 2.0f);
            *index = ct;
        }
    }
    return currMin;
}

#endif