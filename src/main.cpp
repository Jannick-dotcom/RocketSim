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

using namespace std;

struct vals currentValues;
struct vals *temp = &currentValues;

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
}

void output()
{
    while (notdone)
    {
#ifndef asFastAsPossible
        sem_wait(&sem1);
#endif
        system("clear");
        printVals(temp);
    }
}

int suicideBurnActive = 0;
int entryBurnActive = 0;
void autoland(struct vals *values)
{
    double netAcceleration = values->accVehicle + values->g;
    double tSuicide = -values->spdy / netAcceleration;
    double dSuicide = (tSuicide * tSuicide * netAcceleration) / 2;

    if (values->alt < 70000 && values->spdy < -500 && entryBurnActive == 0)
    {
        values->throttle = 1;
        entryBurnActive = 1;
        temp->ctEngines = 3;
    }
    else if (values->spdy >= -400 && entryBurnActive == 1)
    {
        values->throttle = 0;
        entryBurnActive = 2;
        temp->ctEngines = 1;
        values->aSuicideTarget = (temp->engThrust * temp->ctEngines) / temp->vehMass;
    }
    else if (((dSuicide >= values->alt && entryBurnActive == 2) || suicideBurnActive) && values->alt < 100000 && values->alt > 0)
    {
        if (values->spdy > -3.0)
        {
            values->aSuicideTarget = 0;
        }
        double realAccel = (values->accVehicle * temp->throttle) + values->g + values->accDragY;
        double engineAccelReq = values->aSuicideTarget - values->g - values->accDragY;
        values->throttle = engineAccelReq / values->accVehicle;
        // values->throttle = 1;
        suicideBurnActive = 1;
    }
    else if (values->alt < 0)
    {
        suicideBurnActive = 0;
        values->throttle = 0.0;
    }
}

void executeFlightPath()
{
    init(temp);
//     temp->throttle = 1.0;
//     temp->ctEngines = 9;
//     while (temp->alt < 60000.0)
//     {
// #ifndef asFastAsPossible
//         sem_wait(&sem2);
// #endif
//         doStep(temp);
//     }
//     temp->throttle = 0.0;
//     while (temp->spdy >= 0)
//     {
// #ifndef asFastAsPossible
//         sem_wait(&sem2);
// #endif
//         doStep(temp);
//     }
    temp->alt = 60000;
    temp->spdy = -400;
    temp->vehMass = 25000;
    entryBurnActive = 2;
    temp->ctEngines = 1;
    doStep(temp);
    temp->aSuicideTarget = temp->accVehicle + temp->g;
    while (temp->alt > 0 && temp->alt < INFINITY)
    {
        autoland(temp);
#ifndef asFastAsPossible
        sem_wait(&sem2);
#endif
        doStep(temp);
    }
    printVals(temp);
    notdone = false;
    exit(0);
}

void startThreads()
{
    const uint16_t threads = 3; //10
    std::thread tmp[threads];
    tmp[0] = std::thread(executeFlightPath);
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

int main()
{
    sem_init(&sem1, 0, 0);
    sem_init(&sem2, 0, 0);
    startThreads();
    semctl(semid1, 0, IPC_RMID, 0);
    semctl(semid2, 0, IPC_RMID, 0);
    return 0;
}