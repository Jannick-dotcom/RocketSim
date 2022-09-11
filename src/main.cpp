#include <iostream>
#include <fstream>
#include <unistd.h>
#include <math.h>
#include <thread>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <iostream>

#include "mainGame.h"
#include "helpers.h"

static int semid1, semid2; // semaphore ids
sem_t sem1; // semaphore for mainGame
sem_t sem2; // semaphore for mainGame

volatile bool notdone = true; 
struct vals *tempOut; // tempOut is a pointer to a struct of type vals

using namespace std; 
ofstream f("test.txt", std::ios_base::app); // open file for writing

double calculate_d_suicide(struct vals *values, uint8_t ct_engines)
{
    if(ct_engines == 0)
    {
        ct_engines = values->ctEngines;
    }
    double temp_accVehicle = (values->engThrust * ct_engines) / values->vehMass;
    double temp_netAcceleration = temp_accVehicle + values->g; //Max Possible Acceleration
    double temp_speedVector = sqrt(pow(values->spdy, 2) + pow(values->spdx, 2));
    double temp_tSuicide = temp_speedVector / temp_netAcceleration;
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

            if(dSuicide > 900 && dSuicide < 2000)
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
    double angleRetrograde = rad2deg(atan(values->spdx / values->spdy));
    values->angle = angleRetrograde;
    if(values->entryBurnActive == 0 && values->alt < 6e4 && values->spdy < -1000)
    {
        values->throttle = 1.0;
        values->entryBurnActive = 1;
    }
    else if(values->entryBurnActive == 1 && values->alt < 6e4 && values->spdy > -900)
    {
        values->throttle = 0.0;
        values->entryBurnActive = 2;
    }

    if (values->alt <= 0)
    {
        values->suicideBurnActive = 0;
        values->throttle = 0.0;
    }
    else if ((dSuicide >= values->alt && values->alt < 10000 && values->alt > 0 && values->spdy < 0) || values->suicideBurnActive)
    {
        // logfile << logValsToCsv(values);
        values->throttle = dSuicide / values->alt;
        if(values->throttle > 1 && values->ctEngines < 9)
        {
            values->ctEngines++;
        }
        else if(values->throttle <= 0.2 && values->ctEngines > 1)
        {
            values->ctEngines--;
        }
        // values->throttle = 1;
        values->suicideBurnActive = 1;
        // values->stepsize = 0.0001;
    }
}

void writeToFile(struct vals *values, double val)
{
    sem_wait(&sem1);
    f << val << ", " << values->alt << ", " << values->spdy << ", " << values->vehMass << endl;
    sem_post(&sem1);
}

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
        system("clear");
        if(tempOut != nullptr) 
        {
            printVals(tempOut);
        }
#ifndef asFastAsPossible
        sem_wait(&sem1);
#endif
    }
}

void executeFlightPath(double valToVariate)
{
    struct vals currentValues;
    struct vals *temp = &currentValues;
    tempOut = temp;
    int pitchManeuverFin = 0;
    notdone = true;
    init(temp);
    temp->throttle = 1.0;
    temp->ctEngines = 9;
    while (temp->spdx < valToVariate && temp->spdy >= -1)
    {
#ifndef testing
#ifndef asFastAsPossible
        sem_wait(&sem2);
#endif
#endif
        if(temp->spdy > 10.0 && temp->spdy < 500.0 && pitchManeuverFin == 0)
        {
            temp->angle = 10.0;
        }
        else if((temp->spdy != 0 && temp->spdy >= 500.0) || pitchManeuverFin)
        {
            pitchManeuverFin = 1;
            // double anglePrograde = rad2deg(atan(temp->spdx / temp->spdy));
            double desiredAngle = 90.0*(exp(temp->alt / 150000.0) - 1);
            if(desiredAngle > 90.0) desiredAngle = 90.0;
            temp->angle = desiredAngle;
        }
        doStep(temp);
    }
    temp->throttle = 0.0;
    temp->ctEngines = 3;
    while (temp->spdy >= 0)
    {
#ifndef testing
#ifndef asFastAsPossible
        sem_wait(&sem2);
#endif
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
#ifndef testing
#ifndef asFastAsPossible
        sem_wait(&sem2);
#endif
#endif
        temp->angle = 0;
        doStep(temp);
    }
    
    while (temp->alt > 0 && temp->spdy < 0)
    {
        autoland(temp);
#ifndef testing
#ifndef asFastAsPossible
        sem_wait(&sem2);
#endif
#endif
        doStep(temp);
    }
    notdone = false;
#ifdef testing
    writeToFile(temp, valToVariate);
#else
    system("clear");
    printVals(temp);
#endif
}

void startGUIThreads()
{
#ifndef asFastAsPossible
    const uint16_t threads = 3; //10
#else
    const uint16_t threads = 1;
#endif
    std::thread tmp[threads];
    tmp[0] = std::thread(executeFlightPath, 13360);
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
    const uint16_t threads = std::thread::hardware_concurrency(); //10
    std::thread tmp[threads];
    sem_post(&sem1);
    uint8_t running_threads = 0;
    for (uint32_t i = 12700; i < 13000; i++)
    {
        tmp[running_threads] = std::thread(executeFlightPath, double(i)); 
        pthread_setname_np(tmp[running_threads].native_handle(), "calc");
        running_threads++;
        if(running_threads >= threads)
        {
            for (uint16_t thr = 0; thr < running_threads; thr++)
            {
                tmp[thr].join();
            }
            running_threads = 0;
        }
    }
}

int main()
{
    sem_init(&sem1, 0, 0);
    sem_init(&sem2, 0, 0);
#ifdef testing
    startNoGUIThreads();
#else
    startGUIThreads();
#endif
    semctl(semid1, 0, IPC_RMID, 0);
    semctl(semid2, 0, IPC_RMID, 0);
    f.close();
    logfile.close();
    return 0;
}