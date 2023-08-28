/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 3
        Date of Submission: 8/29/2023
        Name of this file: mutexes.c
        Short Description of contents:
            This file contains the getters/setters for the global variables that require semaphore protection
*/

#include <semaphore.h>
#include <stdbool.h>
#include "mutexes.h"

/*
    Function Name: initializeMutexes

    Input to Method:
        N/A

    Output (Return value):
        N/A (void)

    Brief description of the task:
        Initilize all mutexes to 1
*/
void initializeMutexes()
{
    // Initialize all mutexes to 1
    sem_init(&currentFloorMutex, 0, 1);
    sem_init(&peopleCountMutex, 0, 1);
    sem_init(&numFloorsMutex, 0, 1);
    sem_init(&directionMutex, 0, 1);
    sem_init(&openDoorsMutex, 0, 1);
    sem_init(&wanderTimeMutex, 0, 1);
    sem_init(&elevatorRosterMutex, 0, 1);
    sem_init(&requestFloorMutex, 0, 1);
    sem_init(&waitingAtFloorMutex, 0, 1);
}

/*
    Function Name: setElevatorDirection

    Input to Method:
        int direction - Direction to set elevatorDirectionGlobal to

    Output (Return value):
        N/A (void)

    Brief description of the task:
        Semaphore protection to update the elevatorDirectionGlobal variable
*/
void setElevatorDirection(int direction) 
{
    // Wait for access to elevatorDirectionGlobal
    sem_wait(&directionMutex);
    // Update elevatorDirectionGlobal
    elevatorDirectionGlobal = direction;
    // Release access to elevatorDirectionGlobal
    sem_post(&directionMutex);
}

/*
    Function Name: getElevatorDirection

    Input to Method:
        N/A

    Output (Return value):
        int - The direction the elevator is traveling

    Brief description of the task:
        Semaphore protection to read the elevatorDirectionGlobal variable
*/
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

/*
    Function Name: setNumFloors

    Input to Method:
        int numFloors - Number of floors to set numFloorsGlobal to

    Output (Return value):
        N/A (void)

    Brief description of the task:
        Semaphore protection to update the numFloorsGlobal variable
*/
void setNumFloors(int numFloors) 
{
    // Wait for access to numFloorsGlobal
    sem_wait(&numFloorsMutex);
    // Update numFloorsGlobal
    numFloorsGlobal = numFloors;
    // Release access to numFloorsGlobal
    sem_post(&numFloorsMutex);
}

/*
    Function Name: getNumFloors

    Input to Method:
        N/A

    Output (Return value):
        int - Total number of floors

    Brief description of the task:
        Semaphore protection to read the numFloorsGlobal variable
*/
int getNumFloors() 
{
    // Wait for access to numFloorsGlobal
    sem_wait(&numFloorsMutex);
    // Get numFloorsGlobal
    int numFloors = numFloorsGlobal;
    // Release access to numFloorsGlobal
    sem_post(&numFloorsMutex);
    return numFloors;
}

/*
    Function Name: setPeopleCount

    Input to Method:
        int count - Number of people to set peopleCountGlobal to

    Output (Return value):
        N/A (void)

    Brief description of the task:
        Semaphore protection to update the peopleCountGlobal variable
*/
void setPeopleCount(int count) 
{
    // Wait for access to peopleCountGlobal
    sem_wait(&peopleCountMutex);
    // Update peopleCountGlobal
    peopleCountGlobal = count;
    // Release access to peopleCountGlobal
    sem_post(&peopleCountMutex);
}

/*
    Function Name: getPeopleCount

    Input to Method:
        N/A

    Output (Return value):
        int - Total number of people

    Brief description of the task:
        Semaphore protection to read the peopleCountGlobal variable
*/
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

/*
    Function Name: setCurrentFloor

    Input to Method:
        int newFloor - The floor to update the currentFloorGlobal variable to

    Output (Return value):
        N/A (void)

    Brief description of the task:
        Semaphore protection to update the currentFloorGlobal variable
*/
void setCurrentFloor(long newFloor) 
{
    // Wait for access to currentFloor
    sem_wait(&currentFloorMutex);
    // Update currentFloor
    currentFloorGlobal = newFloor;
    // Release access to currentFloor
    sem_post(&currentFloorMutex);
}

/*
    Function Name: getCurrentFloor

    Input to Method:
        N/A

    Output (Return value):
        int - The current floor that the elevator is at

    Brief description of the task:
        Semaphore protection to read the currentFloorGlobal variable
*/
long getCurrentFloor() 
{
    // Wait for access to currentFloor
    sem_wait(&currentFloorMutex);
    // Get currentFloor
    long currentFloor = currentFloorGlobal;
    // Release access to currentFloor
    sem_post(&currentFloorMutex);
    return currentFloor;
}

/*
    Function Name: setDoorsOpen

    Input to Method:
        bool open - The value to update the openDoorsGlobal variable to

    Output (Return value):
        N/A (void)

    Brief description of the task:
        Semaphore protection to update the openDoorsGlobal variable
*/
void setDoorsOpen(bool open) 
{
    // Wait for access to openDoorsGlobal
    sem_wait(&openDoorsMutex);
    // Update openDoorsGlobal
    openDoorsGlobal = open;
    // Release access to openDoorsGlobal
    sem_post(&openDoorsMutex);
}

/*
    Function Name: getDoorsOpen

    Input to Method:
        N/A

    Output (Return value):
        bool - The state of the elevator doors (open or closed)

    Brief description of the task:
        Semaphore protection to read the openDoorsGlobal variable
*/
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

/*
    Function Name: setWanderingTime

    Input to Method:
        int time - The value to update the wanderingTimeGlobal variable to

    Output (Return value):
        N/A (void)

    Brief description of the task:
        Semaphore protection to update the wanderingTimeGlobal variable
*/
void setWanderingTime(int time) 
{
    // Wait for access to wanderingTimeGlobal
    sem_wait(&wanderTimeMutex);
    // Update wanderingTimeGlobal
    wanderingTimeGlobal = time;
    // Release access to wanderingTimeGlobal
    sem_post(&wanderTimeMutex);
}

/*
    Function Name: getWanderingTime

    Input to Method:
        N/A

    Output (Return value):
        int - The max wander time

    Brief description of the task:
        Semaphore protection to read the wanderingTimeGlobal variable
*/
int getWanderingTime() 
{
    // Wait for access to wanderingTimeGlobal
    sem_wait(&wanderTimeMutex);
    // Get wanderingTimeGlobal
    int time = wanderingTimeGlobal;
    // Release access to wanderingTimeGlobal
    sem_post(&wanderTimeMutex);
    return time;
}

/*
    Function Name: getIsOnElevator

    Input to Method:
        int personId - The person id to lookup on the elevator

    Output (Return value):
        bool - if the given personId on the elevator

    Brief description of the task:
        Semaphore protection to read the elevatorRosterGlobal array and check if the given personId is on the elevator
*/
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

/*
    Function Name: boardElevator

    Input to Method:
        int personId - The person id to board on the elevator

    Output (Return value):
        bool - if the given personId was successfully boarded on the elevator

    Brief description of the task:
        Semaphore protection to update the elevatorRosterGlobal array and board a person onto the elevator
*/
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

/*
    Function Name: leaveElevator

    Input to Method:
        int personId - The person id to deboard on the elevator

    Output (Return value):
        bool - if the given personId was successfully deboarded from the elevator

    Brief description of the task:
        Semaphore protection to update the elevatorRosterGlobal array and deboard a person from the elevator
*/
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

/*
    Function Name: isFloorRequested

    Input to Method:
        long floor - The floor number to check if it is requested

    Output (Return value):
        bool - if the given floor is requested

    Brief description of the task:
        Semaphore protection to read the requestFloorGlobal array and check if the given floor is requested
*/
bool isFloorRequested(long floor)
{
    // Wait for access to requestFloorGlobal
    sem_wait(&requestFloorMutex);
    // Get requestFloorGlobal
    bool requested = requestFloorGlobal[floor];
    // Release access to requestFloorGlobal
    sem_post(&requestFloorMutex);
    return requested;
}

/*
    Function Name: isFloorRequested

    Input to Method:
        int floor - The floor number to request

    Output (Return value):
        N/A (void)

    Brief description of the task:
        Semaphore protection to update the requestFloorGlobal array and set the given floor to requested (true)
*/
void requestFloor(int floor)
{
    // Wait for access to requestFloorGlobal
    sem_wait(&requestFloorMutex);
    // Update requestFloorGlobal
    requestFloorGlobal[floor] = true;
    // Release access to requestFloorGlobal
    sem_post(&requestFloorMutex);
}

/*
    Function Name: clearFloorRequest

    Input to Method:
        long floor - The floor number to clear

    Output (Return value):
        N/A (void)

    Brief description of the task:
        Semaphore protection to update the requestFloorGlobal array and clear the call request for the given floor
*/
void clearFloorRequest(long floor)
{
    // Wait for access to requestFloorGlobal
    sem_wait(&requestFloorMutex);
    // Update requestFloorGlobal
    requestFloorGlobal[floor] = false;
    // Release access to requestFloorGlobal
    sem_post(&requestFloorMutex);
}

/*
    Function Name: getWaitingAtAllFloors

    Input to Method:
        N/A

    Output (Return value):
        int* - Array of length n representing how many people are waiting at each floor. (n = number of floors)

    Brief description of the task:
        Semaphore protection to read the waitingAtFloorGlobal array
*/
int *getWaitingAtAllFloors()
{
    // Wait for access to waitingAtFloorGlobal
    sem_wait(&waitingAtFloorMutex);
    // Get waitingAtFloorGlobal
    int *waiting = waitingAtFloorGlobal;
    // Release access to waitingAtFloorGlobal
    sem_post(&waitingAtFloorMutex);
    return waiting;
}

/*
    Function Name: incrementWaitingCount

    Input to Method:
        int floor - The floor number to increment the waiting count

    Output (Return value):
        N/A (void)

    Brief description of the task:
        Semaphore protection to update the waiting count array (waitingAtFloorGlobal) at a given floor
*/
void incrementWaitingCount(int floor)
{
    // Wait for access to waitingAtFloorGlobal
    sem_wait(&waitingAtFloorMutex);
    // Update waitingAtFloorGlobal
    waitingAtFloorGlobal[floor]++;
    // Release access to waitingAtFloorGlobal
    sem_post(&waitingAtFloorMutex);
}

/*
    Function Name: decrementWaitingCount

    Input to Method:
        int floor - The floor number to decrement the waiting count

    Output (Return value):
        N/A (void)

    Brief description of the task:
        Semaphore protection to update the waiting count array (waitingAtFloorGlobal) at a given floor
*/
void decrementWaitingCount(int floor)
{
    // Wait for access to waitingAtFloorGlobal
    sem_wait(&waitingAtFloorMutex);
    // Update waitingAtFloorGlobal
    waitingAtFloorGlobal[floor]--;
    // Release access to waitingAtFloorGlobal
    sem_post(&waitingAtFloorMutex);
}