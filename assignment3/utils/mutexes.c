/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 3
        Date of Submission: 8/99/2023
        Name of this file: person.c
        Short Description of contents:
            This file contains the person data structure
*/

#include <semaphore.h>
#include <stdbool.h>
#include "mutexes.h"

void setelevatorDirectionGlobal(int direction) 
{
    // Wait for access to elevatorDirectionGlobal
    sem_wait(&directionMutex);
    // Update elevatorDirectionGlobal
    elevatorDirectionGlobal = direction;
    // Release access to elevatorDirectionGlobal
    sem_post(&directionMutex);
}

int getElvatorDirection() 
{
    // Wait for access to elevatorDirectionGlobal
    sem_wait(&directionMutex);
    // Get elevatorDirectionGlobal
    int direction = elevatorDirectionGlobal;
    // Release access to elevatorDirectionGlobal
    sem_post(&directionMutex);
    return direction;
}

void setCurrentFloor(long newFloor) 
{
    // Wait for access to currentFloor
    sem_wait(&mutex);
    // Update currentFloor
    currentFloorGlobal = newFloor;
    // Release access to currentFloor
    sem_post(&mutex);
}

long getCurrentFloor() 
{
    // Wait for access to currentFloor
    sem_wait(&mutex);
    // Get currentFloor
    long currentFloor = currentFloorGlobal;
    // Release access to currentFloor
    sem_post(&mutex);
    return currentFloor;
}

void setDoorsOpen(bool open) 
{
    // Wait for access to openDoorsGlobal
    sem_wait(&openDoorsMutex);
    // Update openDoorsGlobal
    openDoorsGlobal = open;
    // Release access to openDoorsGlobal
    sem_post(&openDoorsMutex);
}

bool getDoorsOpen()
{
    // Wait for access to openDoorsGlobal
    sem_wait(&openDoorsMutex);
    // Get openDoorsGlobal
    bool open = openDoorsGlobal;
    // Release access to openDoorsGlobal
    sem_post(&openDoorsMutex);
    return open;
}