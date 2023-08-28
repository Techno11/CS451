/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 3
        Date of Submission: 8/99/2023
        Name of this file: mutexes.c
        Short Description of contents:
            This file contains the getters/setters for the global variables that require semaphore protection
*/

#include <semaphore.h>
#include <stdbool.h>
#include "mutexes.h"

void initializeMutexes()
{
    // Initialize all mutexes to 1
    sem_init(&mutex, 0, 1);
    sem_init(&peopleCountMutex, 0, 1);
    sem_init(&directionMutex, 0, 1);
    sem_init(&openDoorsMutex, 0, 1);
    sem_init(&wanterTimeMutex, 0, 1);
    sem_init(&elevatorRosterMutex, 0, 1);
    sem_init(&requestFloorMutex, 0, 1);
}

void setElevatorDirectionGlobal(int direction) 
{
    // Wait for access to elevatorDirectionGlobal
    sem_wait(&directionMutex);
    // Update elevatorDirectionGlobal
    elevatorDirectionGlobal = direction;
    // Release access to elevatorDirectionGlobal
    sem_post(&directionMutex);
}

int getElevatorDirection() 
{
    // Wait for access to elevatorDirectionGlobal
    sem_wait(&directionMutex);
    // Get elevatorDirectionGlobal
    int direction = elevatorDirectionGlobal;
    // Release access to elevatorDirectionGlobal
    sem_post(&directionMutex);
    return direction;
}

void setPeopleCount(int count) 
{
    // Wait for access to peopleCountGlobal
    sem_wait(&peopleCountMutex);
    // Update peopleCountGlobal
    peopleCountGlobal = count;
    // Release access to peopleCountGlobal
    sem_post(&peopleCountMutex);
}

int getPeopleCount() 
{
    // Wait for access to peopleCountGlobal
    sem_wait(&peopleCountMutex);
    // Get peopleCountGlobal
    int count = peopleCountGlobal;
    // Release access to peopleCountGlobal
    sem_post(&peopleCountMutex);
    return count;
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

void setWanderingTime(int time) 
{
    // Wait for access to wanderingTimeGlobal
    sem_wait(&wanterTimeMutex);
    // Update wanderingTimeGlobal
    wanderingTimeGlobal = time;
    // Release access to wanderingTimeGlobal
    sem_post(&wanterTimeMutex);
}

int getWanderingTime() 
{
    // Wait for access to wanderingTimeGlobal
    sem_wait(&wanterTimeMutex);
    // Get wanderingTimeGlobal
    int time = wanderingTimeGlobal;
    // Release access to wanderingTimeGlobal
    sem_post(&wanterTimeMutex);
    return time;
}

bool getIsOnElevator(int personId)
{
    // Wait for access to elevatorRosterGlobal
    sem_wait(&elevatorRosterMutex);
    int totalPeople = getPeopleCount();

    // Iterate through elevatorRosterGlobal to see if personId is on the elevator
    for (int i = 0; i < totalPeople; i++)
    {
        if (elevatorRosterGlobal[i] == personId)
        {
            // Release access to elevatorRosterGlobal
            sem_post(&elevatorRosterMutex);
            return true;
        }
    }

    // Release access to elevatorRosterGlobal
    sem_post(&elevatorRosterMutex);

    return false;
}

bool boardElevator(int personId)
{
    // Get if personId is on the elevator. If so, return
    if (getIsOnElevator(personId) || !getDoorsOpen())
    {
        return false;
    }

    // Wait for access to elevatorRosterGlobal
    sem_wait(&elevatorRosterMutex);
    int totalPeople = getPeopleCount();
    // Find first -1 in roster and board the elevator
    for (int i = 0; i < totalPeople; i++)
    {
        if (elevatorRosterGlobal[i] == -1)
        {
            elevatorRosterGlobal[i] = personId;
            // Release access to elevatorRosterGlobal
            sem_post(&elevatorRosterMutex);
            return true;
        }
    }

    // Release access to elevatorRosterGlobal
    sem_post(&elevatorRosterMutex);
    return false; // Failed
}

bool leaveElevator(int personId)
{
    // Get if personId is on the elevator. If not, return
    if (!getIsOnElevator(personId) || !getDoorsOpen())
    {
        return false;
    }
    // Wait for access to elevatorRosterGlobal
    sem_wait(&elevatorRosterMutex);
    int totalPeople = getPeopleCount();
    // Find personId in roster and remove them from the elevator
    for (int i = 0; i < totalPeople; i++)
    {
        if (elevatorRosterGlobal[i] == personId)
        {
            elevatorRosterGlobal[i] = -1;
            // Release access to elevatorRosterGlobal
            sem_post(&elevatorRosterMutex);
            return true;
        }
    }

    // Release access to elevatorRosterGlobal
    sem_post(&elevatorRosterMutex);
    return false; // Failed
}

bool isFloorRequested(int floor)
{
    // Wait for access to requestFloorGlobal
    sem_wait(&requestFloorMutex);
    // Get requestFloorGlobal
    bool requested = requestFloorGlobal[floor];
    // Release access to requestFloorGlobal
    sem_post(&requestFloorMutex);
    return requested;
}

void requestFloor(int floor)
{
    // Wait for access to requestFloorGlobal
    sem_wait(&requestFloorMutex);
    // Update requestFloorGlobal
    requestFloorGlobal[floor] = true;
    // Release access to requestFloorGlobal
    sem_post(&requestFloorMutex);
}

void clearFloorRequest(int floor)
{
    // Wait for access to requestFloorGlobal
    sem_wait(&requestFloorMutex);
    // Update requestFloorGlobal
    requestFloorGlobal[floor] = false;
    // Release access to requestFloorGlobal
    sem_post(&requestFloorMutex);
}