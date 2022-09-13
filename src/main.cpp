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

#include "mainGame.hpp"
#include "helpers.hpp"
#include "vectors.hpp"
#include "autoland.hpp"

extern std::ofstream logfile;

static int semid1, semid2; // semaphore ids
sem_t sem1; // semaphore for mainGame
sem_t sem2; // semaphore for mainGame

volatile bool notdone = true; 
struct vals *tempOut; // tempOut is a pointer to a struct of type vals

using namespace std; 
ofstream f("outputs/test.txt"); // open file for writing

void writeToFile(struct vals *values, double val)
{
    sem_wait(&sem1);
    f << val << ", " << values->alt << ", " << values->speed.getlength() << ", " << values->vehMass << endl;
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
    notdone = true;
    init(temp);
    temp->throttle = 1.0;
    temp->ctEngines = 9;
    while (temp->speed.getlength() < valToVariate && temp->speed.gety() >= -1)
    {
#ifndef testing
#ifndef asFastAsPossible
        sem_wait(&sem2);
#endif
#endif
        double desiredAngle = 90.0*(exp(temp->alt / 150000.0) - 1);
        if(desiredAngle > 90.0) desiredAngle = 90.0;
        else if(desiredAngle < 0.0) desiredAngle = 0.0;
        temp->angle = desiredAngle;
        doStep(temp);
    }
    temp->throttle = 0.0;
    temp->ctEngines = 3;
    while (temp->alt > 0)
    {
        if(temp->alt < 100000 && temp->alt < INFINITY)
        {
            autoland(temp);
        }
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
    tmp[0] = std::thread(executeFlightPath, 7700);
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
    for (uint32_t i = 1000; i < 7500; i++)
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