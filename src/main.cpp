#include <iostream>
#include <fstream>
#include <unistd.h>
#include <math.h>
#include <thread>
#include <pthread.h>
#include "mainGame.h"
#include "helpers.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <semaphore.h>

// #define asFastAsPossible

static int semid1, semid2;
sem_t sem1;
sem_t sem2;

volatile bool notdone = true;
struct vals *tempOut;
struct vals currentValues;
using namespace std;

void timing()
{
    uint16_t refreshOut = 20;
    uint16_t refreshCalc = 1000;
    uint8_t timeWarp = 20;
    uint64_t counterOut = 0;
    uint64_t counterUpdate = 0;
    while (notdone)
    {
        if (counterOut >= (10000.0 / (refreshOut)))
        {
            sem_post(&sem1);
            counterOut = 0;
        }
        if (counterUpdate >= (10000.0 / (refreshCalc * timeWarp)))
        {
            sem_post(&sem2);
            counterUpdate = 0;
        }
        usleep(100);
        counterOut++;
        counterUpdate++;
    }
    sem_post(&sem1);
}

void output()
{
    while (notdone)
    {
#ifndef asFastAsPossible
        sem_wait(&sem1);
#endif
        system("clear");
        if(tempOut != nullptr) printVals(tempOut);
    }
}

void autoland(struct vals *values)
{
    double netAcceleration = values->accVehicle + values->g; //Max Possible Acceleration
    double speedVector = sqrt(pow(values->spdy, 2) + pow(values->spdx, 2));
    double tSuicide = speedVector / netAcceleration;
    double dSuicide = (tSuicide * tSuicide * netAcceleration) / 2; // S = 1/2 * a * t²
    double angleRetrograde = rad2deg(atan(values->spdx / values->spdy));
    values->angle = angleRetrograde;
    if (values->alt <= 0)
    {
        values->suicideBurnActive = 0;
        values->throttle = 0.0;
    }
    else if ((dSuicide >= values->alt && values->alt < 10000 && values->alt > 0) || values->suicideBurnActive)
    {
        values->throttle = dSuicide / values->alt;
        values->suicideBurnActive = 1;
        values->stepsize = 0.0001;
    }
}

void executeFlightPath(double valToVariate)
{
    struct vals *temp = &currentValues;
    tempOut = temp;
    int pitchManeuverFin = 0;
    notdone = true;
    init(temp);
    temp->throttle = 1.0;
    temp->ctEngines = 9;
    while (temp->spdx < 6000)
    {
#ifndef asFastAsPossible
        // sem_wait(&sem2);
#endif
        if(temp->spdy > 10.0 && temp->spdy < 300.0 && pitchManeuverFin == 0)
        {
            temp->angle = 10.0;
        }
        else if((temp->spdy != 0 && temp->spdy >= 300.0) || pitchManeuverFin)
        {
            pitchManeuverFin = 1;
            // double anglePrograde = rad2deg(atan(temp->spdx / temp->spdy));
            double desiredAngle = 90.0*(exp(temp->alt / 100000.0) - 1);
            if(desiredAngle > 90.0) desiredAngle = 90.0;
            temp->angle = desiredAngle;
        }
        doStep(temp);
    }
    temp->throttle = 0.0;
    temp->ctEngines = 1;
    while (temp->spdy >= 0)
    {
#ifndef asFastAsPossible
        // sem_wait(&sem2);
#endif
        // double anglePrograde = rad2deg(atan(temp->spdx / temp->spdy));
        if(temp->spdy < 25)
        {
            double orbitalSpeed = sqrt(temp->gravConst * temp->earthMass / (temp->alt + temp->earthRadius));
            if(temp->spdx < orbitalSpeed-5)
            {
                temp->angle = 90;
                temp->throttle = 1;
            }
            else
            {
                temp->throttle = 0;
                break;
            }
        }
        else
        {
            temp->angle = 0;
        }
        doStep(temp);
    }
    temp->throttle = 0;
    while (temp->alt > 100000 && temp->alt < INFINITY)
    {
#ifndef asFastAsPossible
        // sem_wait(&sem2);
#endif
        temp->angle = 0;
        doStep(temp);
    }
    temp->ctEngines = 1;
    while (temp->alt > 0)
    {
        autoland(temp);
#ifndef asFastAsPossible
        sem_wait(&sem2);
#endif
        doStep(temp);
    }
    // system("clear");
    // printVals(temp);
    notdone = false;
}

void startGUIThreads()
{
#ifndef asFastAsPossible
    const uint16_t threads = 3; //10
#else
    const uint16_t threads = 1;
#endif
    std::thread tmp[threads];
    tmp[0] = std::thread(executeFlightPath, 0);
#ifndef asFastAsPossible
    tmp[1] = std::thread(output);
    tmp[2] = std::thread(timing);
#endif
    pthread_setname_np(tmp[0].native_handle(), "calc");
#ifndef asFastAsPossible
    pthread_setname_np(tmp[1].native_handle(), "out");
    pthread_setname_np(tmp[2].native_handle(), "timing");
#endif
    tmp[0].join();
#ifndef asFastAsPossible
    tmp[1].join();
    tmp[2].join();
#endif
}

void startNoGUIThreads()
{
    const uint16_t threads = 12; //10
    std::thread tmp[threads];
    for (uint8_t thr = 0; thr < threads; thr++)
    {
        tmp[thr] = std::thread(executeFlightPath, 0);
        pthread_setname_np(tmp[thr].native_handle(), "calc");
        // tmp[thr].detach();
    }
    for (uint8_t thr = 0; thr < threads; thr++)
    {
        // pthread_setname_np(tmp[thr].native_handle(), "calc");
        tmp[thr].join();
    }
}

int main()
{
    sem_init(&sem1, 0, 0);
    sem_init(&sem2, 0, 0);
    // for(uint8_t i = 0; i < 10; i++)
    // {
    //     startNoGUIThreads();
    // }
    startGUIThreads();
    semctl(semid1, 0, IPC_RMID, 0);
    semctl(semid2, 0, IPC_RMID, 0);
    return 0;
}