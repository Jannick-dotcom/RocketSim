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
volatile double lastspeed = 0;
int count = 0;
void autoland(struct vals *values)
{
    double accelPresent;
    if(lastspeed == 0)
    {
        lastspeed = values->spdy;
    }
    
    if(count >= 10)
    {
        count = 0;
        accelPresent = (values->spdy - lastspeed) / (values->stepsize * 10); //Measure change in Velocity
        lastspeed = values->spdy;
    }
    count++;
    double netAcceleration = values->accVehicle + values->g; //Max Possible Accelleration
    double tSuicide = -values->spdy / netAcceleration;
    double dSuicide = (tSuicide * tSuicide * netAcceleration) / 2; // S = 1/2 * a * t²

    if (values->alt < 90000 && values->spdy < -1000 && entryBurnActive == 0)
    {
        values->throttle = 1;
        entryBurnActive = 1;
        temp->ctEngines = 3;
    }
    else if (values->spdy >= -900 && entryBurnActive == 1)
    {
        values->throttle = 0;
        entryBurnActive = 2;
        temp->ctEngines = 3;
    }
    else if (((dSuicide >= values->alt && entryBurnActive == 2) || suicideBurnActive) && values->alt < 100000 && values->alt > 0)
    {
        values->throttle = dSuicide / values->alt;
        suicideBurnActive = 1;
    }
    else if (values->alt < 0)
    {
        suicideBurnActive = 0;
        values->throttle = 0.0;
    }
}

void executeFlightPath(double valToVariate)
{
    notdone = true;
    init(temp);
    temp->throttle = 1.0;
    temp->ctEngines = 9;
    while ((temp->vehMass - temp->dryMass) / (temp->initialMass / 100) > 50)
    {
#ifndef asFastAsPossible
        // sem_wait(&sem2);
#endif
        doStep(temp);
    }
    temp->throttle = 0.0;
    while (temp->spdy >= 0)
    {
#ifndef asFastAsPossible
        // sem_wait(&sem2);
#endif
        doStep(temp);
    }
    while (temp->alt > 100000 && temp->alt < INFINITY)
    {
#ifndef asFastAsPossible
        // sem_wait(&sem2);
#endif
        doStep(temp);
    }
    while (temp->alt > 0)
    {
        autoland(temp);
#ifndef asFastAsPossible
        sem_wait(&sem2);
#endif
        doStep(temp);
    }
    system("clear");
    printVals(temp);
    notdone = false;
}

void startGUIThreads()
{
    const uint16_t threads = 3; //10
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
        // pthread_setname_np(tmp[thr].native_handle(), "calc");
        tmp[thr].detach();
    }
}

int main()
{
    sem_init(&sem1, 0, 0);
    sem_init(&sem2, 0, 0);
    startGUIThreads();
    semctl(semid1, 0, IPC_RMID, 0);
    semctl(semid2, 0, IPC_RMID, 0);
    return 0;
}