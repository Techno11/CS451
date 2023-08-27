/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 3
        Date of Submission: 8/99/2023
        Name of this file: elevator.c
        Short Description of contents:
            Main part of elevator program, reads input and kicks off all child threads
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "structs/floor.c"
#include "structs/person.c"
#include "structs/itinerary.c"
#include "structs/queue.c"
#include "utils/setupHelper.c"
#include "utils/elevatorTools.c"
#include <semaphore.h>
#include <pthread.h>

// Constants
#define ELEVATOR_GOING_UP 1
#define ELEVATOR_GOING_DOWN 0

// GLOBAL VARIABLES:
sem_t mutex; // a semaphore global variable called mutex, a binary semaphore used for locking
long currentFloorNum;

sem_t directionMutex;
int elevatorDirection;

sem_t openDoorsMutex;
bool openDoors;

// Set of global queues to use for directing which groups of Persons get off on next floor:
//  -NOTE: Some people might not all disembark from the disembark queue; only got 1 second...
struct Queue *elevatorQueue; // this is the overall group of people riding the elevator
struct Queue *toKeepOnElevator; // this is the group who WON'T be getting off on next floor
struct Queue *disembarkFromElevator; // this is the group who WILL be getting off on next floor

void setElevatorDirection(int direction) 
{
    // Wait for access to elevatorDirection
    sem_wait(&directionMutex);
    // Update elevatorDirection
    elevatorDirection = direction;
    // Release access to elevatorDirection
    sem_post(&directionMutex);
}

int getElvatorDirection() 
{
    // Wait for access to elevatorDirection
    sem_wait(&directionMutex);
    // Get elevatorDirection
    int direction = elevatorDirection;
    // Release access to elevatorDirection
    sem_post(&directionMutex);
    return direction;
}

void setCurrentFloor(long newFloor) 
{
    // Wait for access to currentFloor
    sem_wait(&mutex);
    // Update currentFloor
    currentFloorNum = newFloor;
    // Release access to currentFloor
    sem_post(&mutex);
}

long getCurrentFloor() 
{
    // Wait for access to currentFloor
    sem_wait(&mutex);
    // Get currentFloor
    long currentFloor = currentFloorNum;
    // Release access to currentFloor
    sem_post(&mutex);
    return currentFloor;
}

void setDoorsOpen(bool open) 
{
    // Wait for access to openDoors
    sem_wait(&openDoorsMutex);
    // Update openDoors
    openDoors = open;
    // Release access to openDoors
    sem_post(&openDoorsMutex);
}

bool getDoorsOpen()
{
    // Wait for access to openDoors
    sem_wait(&openDoorsMutex);
    // Get openDoors
    bool open = openDoors;
    // Release access to openDoors
    sem_post(&openDoorsMutex);
    return open;
}

void elevator(long currentFloor)
{   
    // Variable to track things locally, so we don't have to read them from the global variables every time:
    long currentFloorLocal = getCurrentFloor();
    bool openDoorsLocal = getDoorsOpen();
    int elevatorDirectionLocal = getElvatorDirection();

    // The "currentFloor" currently says the top floor, so we need to record that and then reset it to 0:
    long maxFloor = currentFloorLocal - 1;

    // Set the current floor to 0:
    setCurrentFloor(0);
    currentFloorLocal = 0;

    while(true)
    {
        // If we're at the min floor, change direction and print
        if(currentFloorLocal == 0)
        {
            // Print where we're at
            printf("    Elevator: At floor %ld.\n", currentFloorLocal);
            printf("    Elevator: Heading to max Floor %ld\n", maxFloor);
            setElevatorDirection(ELEVATOR_GOING_UP);
            elevatorDirectionLocal = ELEVATOR_GOING_UP;
            
        }
        // If we're at the max floor, change direction and print
        else if (currentFloorLocal == maxFloor)
        {
            // Print where we're at
            printf("    Elevator: At floor %ld.\n", currentFloorLocal);
            printf("    Elevator: Heading to min Floor %d\n", 0);
            setElevatorDirection(ELEVATOR_GOING_DOWN);
            elevatorDirectionLocal = ELEVATOR_GOING_DOWN;
        }

        // If someone is waiting on this floor, we need to open the doors
        if (false) // TODO:
        {
            setDoorsOpen(true);
            openDoorsLocal = true;
            printf("    Elevator: Opening the doors at %ld\n", currentFloorLocal);
            // TODO: Get on the elevator
            
        }

        sleep(1);

        // Go up (or down) a floor
        if (elevatorDirectionLocal == ELEVATOR_GOING_UP)
        {
            currentFloorLocal++;
            setCurrentFloor(currentFloorLocal);
        }
        else
        {
            currentFloorLocal--;
            setCurrentFloor(currentFloorLocal);
        }
    }


    printf("    Elevator: I am about to stop, with a current floor of %ld.\n", currentFloor);

    // ...

    // This is one of the ways to exit a thread:
    //  -Here, it specifies an exit status value of 0
    //  which is available to another thread in the same
    //  process that called pthread_join().
    //      -AKA, just as a fun fact, the person threads or main() can be
    //      given the elevator's thread exit status.
    pthread_exit(0);
}

void person(Person* thisPerson)
{
    printf("Person Number %d: I have entered the building.\n", getPersonKey(thisPerson));

    // ...
    /* while(notDone) {
     *  sleep(wanderTime);
     *  wait for the elevator to come to current floor
     *  get on the elevator;
     *  figure out the next floor to go to;
     *  wait for elevator to reach there;
     *  get off the elevator;
     * }
    */
    // When done print goodbye message and exit thread:
    printf("Person Number %d: Leaving the system, goodbye!\n", getPersonKey(thisPerson));
    pthread_exit(0);
}

/* NOTE: To run the current code from terminal:
 -----------------------------------------------------------------------------------------
   /assignment3$ gcc elevator.c -o elevator.o -lpthread
   /assignment3$ ./elevator.o -p 5 -f 11 -w 10 < ./examples/deterministic_example_2.txt
 -----------------------------------------------------------------------------------------
*/

int main(int argc, char *argv[])
{
    // Initialize any global variables at the start:
    currentFloorNum = 0;
    sem_init(&mutex, 0, 1); // Initialize semaphore mutex to 1

    // Initialize the directionMutex to 1:
    elevatorDirection = ELEVATOR_GOING_UP;
    sem_init(&directionMutex, 0, 1);

    // Initialize the openDoorsMutex to 1:
    openDoors = false;
    sem_init(&openDoorsMutex, 0, 1);

    // Parse input parameters
    int *parsed = malloc(sizeof(int) * 3);
    parseParameters(parsed, argc, argv);
    int passengerCount = parsed[0];
    int wanderingTime = parsed[1];
    int floorCount = parsed[2];
    free(parsed);

    // Create our "Building"
    Floor *floors[floorCount];

    // Create our people lookup
    Person *people[passengerCount];

    // Read standard input
    readSetupStdin(floors, people, floorCount, passengerCount);

    // First thing program does is print input from stdin
    printPeople(people, passengerCount);

    // Sanity Checks
    printFloors(floors, floorCount);

    // Create the elevator queue-array (this time, the queues will be global):
    elevatorQueue = initQueueStruct(elevatorQueue, passengerCount);
    newQueue(elevatorQueue, passengerCount);

    // Set the current floor the the max floor.  The elevator pthread then knows what the max floor is, and will reset it to 0
    setCurrentFloor(floorCount - 1);

    // Create the elevator thread:
    pthread_t elevatorThread;
    pthread_create(&elevatorThread, NULL, (void *(*)(void *)) elevator, (void *)currentFloorNum);

    // Create a person thread for each Person:
    pthread_t personThreads[passengerCount];
    // The following code creates P-amount of threads:
    for (int personIndex = 0; personIndex < passengerCount; personIndex++)
    {
        // Get corresponding Person object to use inside person's thread:
        Person *thisPerson = people[personIndex];

        // Will create a thread:
        pthread_create(&personThreads[personIndex], NULL, (void *(*)(void *))person,  (void *)thisPerson);
    }

    // Join the threads together with the calling main thread so that they have to wait for each other:
    pthread_join(elevatorThread, NULL);
    for (int personIndex = 0; personIndex < passengerCount; personIndex++)
    {
        pthread_join(personThreads[personIndex], NULL);
    }

    // Cleanup
    freeFloors(floors, floorCount);
    freePeople(people, passengerCount);

    return 0;
}