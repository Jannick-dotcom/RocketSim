#include <iostream>
#include <fstream>
#include <math.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "mainGame.hpp"
#include "helpers.hpp"
#include "vectors.hpp"
#include "autoland.hpp"

struct vals *tempOut; // tempOut is a pointer to a struct of type vals
static int semid1, semid2; // semaphore ids
sem_t semOut; // semaphore for output
sem_t semPhys; // semaphore for physics
#ifdef testing
sem_t semFile; // semaphore for file writing
#endif

volatile bool notdone = true; 

using namespace std; 

#ifdef testing
void writeToFile(struct vals *values, double val)
{
    sem_wait(&semFile);
    ofstream f(OutputPath + "test.txt", std::ios_base::app); // open file for writing
    f << setprecision(10) << val << ", " << values->alt << ", " << values->speed.getlength() << ", " << ((values->vehMass - values->dryMass) / values->initialMass)*100.0 << endl;
    f.close();
    sem_post(&semFile);
}
#else

//return the difference in microseconds between two timevals
uint32_t timeDifference(struct timeval t1, struct timeval t2)
{
    return abs((t2.tv_sec - t1.tv_sec) * 1000000 + (t2.tv_usec - t1.tv_usec));
}

void timing()
{
    struct timeval curr;
    uint16_t refreshOut = 10;
    uint16_t refreshCalc = 1000;
    uint8_t timeWarp = 1;
    struct timeval lastOut;
    struct timeval lastPhysics;
    while (notdone)
    {
        gettimeofday(&curr, NULL);
        if (timeDifference(lastOut, curr) >= 1e6 / refreshOut)
        {
            sem_post(&semOut);
            lastOut = curr;
        }
            
        if (timeDifference(lastPhysics, curr) >= 1e6 / (refreshCalc * timeWarp))
        {
            sem_post(&semPhys);
            lastPhysics = curr;
        }

        if(tempOut != nullptr && tempOut->alt < 500.0)
        {
            timeWarp = 1;
        }
        else
        {
            timeWarp = 20;
        }
        usleep(10);
    }
    sem_post(&semOut);
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
        sem_wait(&semOut);
#endif
    }
    cout << "END" << endl;
}
#endif

void executeFlightPath(double valToVariate)
{
    struct vals values;
    struct vals *currentValues = &values;
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
    currentValues->throttleSet = 1.0;
    currentValues->ctEngines = 9;
    while (currentValues->speed.getlength() < valToVariate && currentValues->vehMass > (currentValues->dryMass + 10.0))
    {
#ifndef testing
        sem_wait(&semPhys);
#endif
        double desiredAngle = 90.0*(exp(currentValues->alt / 100000.0) - 1);
        if(desiredAngle > 90.0) desiredAngle = 90.0;
        else if(desiredAngle < 0.0) desiredAngle = 0.0;
        currentValues->orientation = vektor(sin(deg2rad(desiredAngle)), cos(deg2rad(desiredAngle)), 0);
        doStep(currentValues);
    }
    currentValues->throttleSet = 0.0;
    currentValues->ctEngines = 1;
    while (currentValues->alt > 0)
    {
        if(currentValues->alt < 100000 && currentValues->alt < INFINITY)
        {
            autoland(currentValues);
        }
#ifndef testing
        sem_wait(&semPhys);
#endif
        doStep(currentValues);
    }
    currentValues->throttleSet = 0.0;
    lastStep();
#ifdef testing
    writeToFile(currentValues, valToVariate);
#else
    notdone = false;
    system("clear");
    printVals(currentValues);
#endif
}

void startGUIThreads()
{
#ifndef testing
    boost::asio::thread_pool pool(3);
    boost::asio::post(pool, boost::bind(output));
    boost::asio::post(pool, boost::bind(timing));
#else
    boost::asio::thread_pool pool(1);
#endif

    boost::asio::post(pool, boost::bind(executeFlightPath, double(2000)));

    pool.join();
}

void startNoGUIThreads()
{
    boost::asio::thread_pool pool(std::thread::hardware_concurrency());
    // Submit a function to the pool.

    for(uint32_t i = 0; i < 100000; i++)
    {
        boost::asio::post(pool, boost::bind(executeFlightPath, double(i/10.0)));
    }
    pool.join();
}

int main()
{
    sem_init(&semOut, 0, 0);
    sem_init(&semPhys, 0, 0);
#ifdef testing
    sem_init(&semFile, 0, 1);
    remove((OutputPath + "test.txt").c_str());
    startNoGUIThreads();
#else
    startGUIThreads();
#endif
    semctl(semid1, 0, IPC_RMID, 0);
    semctl(semid2, 0, IPC_RMID, 0);
    return 0;
}