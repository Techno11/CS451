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
#include <string.h>
#include "structs/floor.c"
#include "structs/person.c"
#include "structs/itinerary.c"
#include "structs/queue.c"
#include "utils/setupHelper.c"
#include "utils/devtools.c"
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h> // for sleep()

// GLOBAL VARIABLES:
sem_t mutex; //a semaphore global variable called mutex, a binary semaphore used for locking
long currentFloorNum;
int wanderingTime;
// Set of global queues to use for directing which groups of Persons get off on next floor:
//  -NOTE: Some people might not all disembark from the disembark queue; only got 1 second...
struct Queue *elevatorQueue; // this is the overall group of people riding the elevator
struct Queue *toKeepOnElevator; // this is the group who WON'T be getting off on next floor
struct Queue *disembarkFromElevator; // this is the group who WILL be getting off on next floor

void elevator(long currentFloor)
{
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

    // Professor's person psuedocode:
    /* while(notDone) {
     *  sleep(wanderTime);
     *  wait for the elevator to come to current floor
     *  get on the elevator;
     *  figure out the next floor to go to;
     *  wait for elevator to reach there;
     *  get off the elevator;
     * }
    */
    // The "notDone" can be a method to check that Person's itinerary is not completed
    while (!isItineraryDone(thisPerson))
    {
        sleep(wanderingTime);

        int currToDoItemIndex = getCurrentItineraryItemInd(thisPerson);
        Itinerary* currentToDoItem = getCurrentItineraryItem(thisPerson);

        // If you are done waiting && you are going up && elevator is going up then "get on"
        //  -Use defined constants (#'s) at the top
        if (currentFloorNum == currentToDoItem->floor)
        {
            // If you are done waiting && you are going up && elevator is going up then "get on"

            // Else If you are done waiting && you are going down && elevator is going down then "get on"

            // Else "get off" the elevator
        }
        // Figure out next floor to go to (update itinerary index)

    }
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

    // Parse input parameters
    int *parsed = malloc(sizeof(int) * 3);
    parseParameters(parsed, argc, argv);
    int passengerCount = parsed[0];
    wanderingTime = parsed[1];
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