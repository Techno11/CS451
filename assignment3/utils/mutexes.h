/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 3
        Date of Submission: 8/99/2023
        Name of this file: mutexes.h
        Short Description of contents:
            This file contains the prototypes for the mutexes functions
*/

#include <stdbool.h>
#include <semaphore.h>

#ifndef MUTEXES_H
#define MUTEXES_H

// GLOBAL MUTEXES VARIABLES:
sem_t mutex; // a semaphore global variable called mutex, a binary semaphore used for locking
long currentFloorGlobal;

sem_t directionMutex;
int elevatorDirectionGlobal;

sem_t openDoorsMutex;
bool openDoorsGlobal;

int wanderingTime;

/*======== SEE 'mutexes.c' FOR FUNCTION HEADER COMMENTS ========*/
void setelevatorDirectionGlobal(int direction);

int getElvatorDirection();

void setCurrentFloor(long newFloor);

long getCurrentFloor();

void setDoorsOpen(bool open);

bool getDoorsOpen();


#endif // MUTEXES_H