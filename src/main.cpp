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
#include <sys/stat.h>
#include <atomic>   

#include "mainGame.hpp"
#include "helpers.hpp"
#include "vectors.hpp"
#include "autoland.hpp"
#include "threadPool.hpp"

// extern std::ofstream logfile;

static int semid1, semid2; // semaphore ids
sem_t sem1; // semaphore for mainGame
sem_t sem2; // semaphore for mainGame

volatile bool notdone = true; 

using namespace std; 
#ifdef testing

void writeToFile(struct vals *values, double val)
{
    #ifdef multithreading
    sem_wait(&sem1);
    #endif
    ofstream f("outputs/test.txt", std::ios_base::app); // open file for writing
    f << val << ", " << values->alt << ", " << values->speed.getlength() << ", " << values->vehMass << endl;
    f.close();
    #ifdef multithreading
    sem_post(&sem1);
    #endif
}
#else
struct vals *tempOut; // tempOut is a pointer to a struct of type vals
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

#endif

void debugLog(string filename, struct vals *values)
{
    #ifdef multithreading
    // sem_wait(&sem1);
    #endif
    ofstream f("/home/jannick/Desktop/outputs/" + filename, std::ios_base::app); // open file for appending
    f << values->speed.getlength() << endl;
    f.close();
    #ifdef multithreading
    // sem_post(&sem1);
    #endif
}

void executeFlightPath(double valToVariate)
{
    struct vals values;
    struct vals *currentValues = &values;//(struct vals*) malloc(sizeof(struct vals));//param.values;
    if(currentValues == nullptr)
    {
        cout << "Error allocating memory" << endl;
        exit(1);
    }
    #ifndef testing
    tempOut = currentValues;
    notdone = true;
    #endif
    init(currentValues);
    currentValues->throttle = 1.0;
    currentValues->ctEngines = 9;
    while (currentValues->speed.getlength() < valToVariate && currentValues->vehMass > currentValues->dryMass)
    {
#ifndef testing
#ifndef asFastAsPossible
        sem_wait(&sem2);
#endif
#endif
        double desiredAngle = 90.0*(exp(currentValues->alt / 120000.0) - 1);
        if(desiredAngle > 90.0) desiredAngle = 90.0;
        else if(desiredAngle < 0.0) desiredAngle = 0.0;
        currentValues->orientation = vektor(sin(deg2rad(desiredAngle)), cos(deg2rad(desiredAngle)), 0);
        // debugLog(to_string(int(param.valToVariate)) + ".csv", currentValues);
        doStep(currentValues);
    }
    currentValues->throttle = 0.0;
    currentValues->ctEngines = 3;
    while (currentValues->alt > 0)
    {
        if(currentValues->alt < 60000 && currentValues->alt < INFINITY)
        {
            autoland(currentValues);
        }
#ifndef testing
#ifndef asFastAsPossible
        sem_wait(&sem2);
#endif
#endif
        doStep(currentValues);
    }
#ifdef testing
    writeToFile(currentValues, valToVariate);
#else
    notdone = false;
    system("clear");
    printVals(currentValues);
#endif
    // free(currentValues);
}

#ifndef testing
void startGUIThreads()
{
#ifndef asFastAsPossible
    const uint16_t threads = 3; //10
#else
    const uint16_t threads = 1;
#endif
    std::thread tmp[threads];
    tmp[0] = std::thread(executeFlightPath, 501.0);
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

#else
void startNoGUIThreads()
{
    sem_post(&sem1);
    boost::asio::thread_pool pool(11);
    // Submit a function to the pool.

    for(uint16_t i = 500; i < 7000; i++)
    {
        boost::asio::post(pool, boost::bind(executeFlightPath, double(i)));
    }
    pool.join();
}
#endif

int main()
{
    mkdir("outputs", 0777);
    sem_init(&sem1, 0, 0);
    sem_init(&sem2, 0, 0);
#ifdef testing
    remove("outputs/test.txt");
    startNoGUIThreads();
#else
    startGUIThreads();
#endif
    semctl(semid1, 0, IPC_RMID, 0);
    semctl(semid2, 0, IPC_RMID, 0);
    #ifndef testing
    // logfile.close();
    #endif
    return 0;
}