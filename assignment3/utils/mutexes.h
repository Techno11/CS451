/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 3
        Date of Submission: 8/29/2023
        Name of this file: mutexes.h
        Short Description of contents:
            This file contains the prototypes for the mutexes methods
*/

#include <stdbool.h>
#include <semaphore.h>

#ifndef MUTEXES_H
#define MUTEXES_H

// GLOBAL MUTEXES VARIABLES:
sem_t currentFloorMutex; // a semaphore global variable called currentFloorMutex, a binary semaphore used for locking
long currentFloorGlobal;

sem_t peopleCountMutex;
int peopleCountGlobal;

sem_t numFloorsMutex;
int numFloorsGlobal;

sem_t directionMutex;
int elevatorDirectionGlobal;

sem_t openDoorsMutex;
bool openDoorsGlobal;

sem_t wanderTimeMutex;
int wanderingTimeGlobal;

sem_t elevatorRosterMutex;
int *elevatorRosterGlobal;

sem_t requestFloorMutex;
bool *requestFloorGlobal;

sem_t waitingAtFloorMutex;
int *waitingAtFloorGlobal;

/*======== SEE 'mutexes.c' FOR FUNCTION HEADER COMMENTS ========*/
void initializeMutexes();

void setElevatorDirectionGlobal(int direction);

int getElevatorDirection();

int getNumFloors();

void setNumFloors(int numFloors);

void setCurrentFloor(long newFloor);

long getCurrentFloor();

void setDoorsOpen(bool open);

bool getDoorsOpen();

void setWanderingTime(int time);

int getWanderingTime();

// "Custom" getters and setters for getting on/off the elevator and detecting if a person is on the elevator
bool getIsOnElevator(int personId);

bool boardElevator(int personId);

bool leaveElevator(int personId);

// "Custom" getters and setters for requesting floors
bool isFloorRequested(long floor);

void requestFloor(int floor);

void clearFloorRequest(long floor);

// "Custom" getters and setters for accessing floor waiting counts
int *getWaitingAtAllFloors();

void incrementWaitingCount(int floor);

void decrementWaitingCount(int floor);

#endif // MUTEXES_H