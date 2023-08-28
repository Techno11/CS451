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
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h> // for sleep()
#include "structs/floor.c"
#include "structs/person.c"
#include "structs/itinerary.c"
#include "structs/queue.c"
#include "utils/setupHelper.c"
#include "utils/elevatorTools.c"
#include "utils/mutexes.c" // Includes mutexes and global variables

// Macros
#define ELEVATOR_GOING_UP 1
#define ELEVATOR_GOING_DOWN 0

// Toggles the extra credit functionality
#define ENABLE_EXTRA_CREDIT false

// GLOBAL VARIABLES
// Set of global queues to use for directing which groups of Persons get off on next floor:
//  -NOTE: Some people might not all disembark from the disembark queue; only got 1 second...
struct Queue *elevatorQueue;         // this is the overall group of people riding the elevator
struct Queue *toKeepOnElevator;      // this is the group who WON'T be getting off on next floor
struct Queue *disembarkFromElevator; // this is the group who WILL be getting off on next floor

void elevator(long currentFloor)
{
    // Variable to track things locally, so we don't have to read them from the global variables every time:
    long currentFloorLocal = getCurrentFloor();
    bool openDoorsGlobalLocal = getDoorsOpen();
    int elevatorDirectionGlobalLocal = getElevatorDirection();

    // The "currentFloor" currently says the top floor, so we need to record that and then reset it to 0:
    long maxFloor = currentFloorLocal - 1;

    // Set the current floor to 0:
    setCurrentFloor(0);
    currentFloorLocal = 0;

    while (true)
    {
        // If we're at the min floor, change direction and print
        if (currentFloorLocal == 0)
        {
            // Print where we're at
            printf("    Elevator: At floor %ld.\n", currentFloorLocal);
            printf("    Elevator: Heading to max Floor %ld\n", maxFloor);
            setElevatorDirectionGlobal(ELEVATOR_GOING_UP);
            elevatorDirectionGlobalLocal = ELEVATOR_GOING_UP;
        }
        // If we're at the max floor, change direction and print
        else if (currentFloorLocal == maxFloor)
        {
            // Print where we're at
            printf("    Elevator: At floor %ld.\n", currentFloorLocal);
            printf("    Elevator: Heading to min Floor %d\n", 0);
            setElevatorDirectionGlobal(ELEVATOR_GOING_DOWN);
            elevatorDirectionGlobalLocal = ELEVATOR_GOING_DOWN;
        }

        // If someone is waiting on this floor, we need to open the doors
        if (isFloorRequested(currentFloorLocal))
        {
            // Open the doors
            setDoorsOpen(true);
            openDoorsGlobalLocal = true;
            // Print that we've stopped
            printf("    Elevator: Opening the doors at %ld\n", currentFloorLocal);
            // Wait for people to get on
            sleep(1);
            // Close the doors
            setDoorsOpen(false);
            clearFloorRequest(currentFloorLocal);
            openDoorsGlobalLocal = false;
        }

        // Transition between floors
        sleep(1);

        // Go up (or down) a floor
        if (elevatorDirectionGlobalLocal == ELEVATOR_GOING_UP)
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

void person(Person *thisPerson)
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

    // Track whether or not we've wandered the current floor or itenerary item
    bool wanderedCurrentFloor = false;

    // Track whether this is our first move and we just entered the building
    bool firstMove = true;

    // While the person's itinerary is not done, keep looping:
    while (!isItineraryDone(thisPerson))
    {
        // Fetch the current itinerary item and the next itinerary item:
        int currToDoItemIndex = getCurrentItineraryItemInd(thisPerson);
        Itinerary *currentToDoItem = getCurrentItineraryItem(thisPerson);
        Itinerary *nextToDoItem = getNextItineraryItem(thisPerson);

        // Get the current floor we're on. If this is our first move, then we're on floor 0
        int theFloorWeAreOn = currentToDoItem->floor;
        if (firstMove)
        {
            theFloorWeAreOn = 0;
            nextToDoItem = currentToDoItem;
        }

        // If we haven't wandered the current floor, then wander it:
        if (!wanderedCurrentFloor)
        {
            if (!firstMove)
            {
                // Get, print, and wait for wandering time:
                int wanderTime = getWanderingTime();
                printf("Person Number %d: Wandering for %d seconds\n", getPersonKey(thisPerson), wanderTime);
                sleep(wanderTime);
            }

            // We've wandered this floor
            wanderedCurrentFloor = true;

            // Print that we're waiting for the elevator
            printf("Person Number %d: Waiting for elevator at floor %d\n", getPersonKey(thisPerson), theFloorWeAreOn);
        }

        // If you are done waiting && you are going up && elevator is going up then "get on" && the doors are open
        //  -Use defined constants (#'s) at the top
        //  -Current itinerary item is where the Person is AT
        //  -NEXT itinerary item is where the Person is GOING (destination floor)
        int currentFloor = getCurrentFloor();
        int elevatorDirection = getElevatorDirection();

        // Calculate keys to getting on/off the elevator
        bool doWeNeedToGoUp = nextToDoItem->floor > currentFloor;                 // Do we need to go up
        bool elevatorAtOurFloor = currentFloor == theFloorWeAreOn;                // Is the elevator at our floor?
        bool elevatorGoingUp = elevatorDirectionGlobal == ELEVATOR_GOING_UP;      // Is the elevator going up?
        bool doorsOpen = getDoorsOpen();                                          // Are the doors open?
        bool elevatorUpAndWeNeedToGoUp = elevatorGoingUp && doWeNeedToGoUp;       // Is the elevator going up and we need to go up?
        bool elevatorDownAndWeNeedToGoDown = !elevatorGoingUp && !doWeNeedToGoUp; // Is the elevator going down and we need to go down?
        bool onElevator = getIsOnElevator(getPersonKey(thisPerson));              // Are we on the elevator?

        // Request elevator to stop at our floor if it's going the correct direction and extra credit is enabled
        if ((elevatorUpAndWeNeedToGoUp || elevatorDownAndWeNeedToGoDown) && !onElevator && ENABLE_EXTRA_CREDIT)
        {
            requestFloor(theFloorWeAreOn);
        }
        // Request elevator to stop at our floor regardless of direction if extra credit is disabled
        else if (!onElevator && !ENABLE_EXTRA_CREDIT)
        {
            requestFloor(theFloorWeAreOn);
        }

        // If the elevator is at our floor, the doors are open, we're not already on the elevator, the elevator is going the correct direction, and extra credit is enabled
        bool extraCreditCondition = elevatorAtOurFloor && doorsOpen && !onElevator && (elevatorUpAndWeNeedToGoUp || elevatorDownAndWeNeedToGoDown) && ENABLE_EXTRA_CREDIT;

        // If the elevator is at our floor, the doors are open, we're not already on the elevator, and extra credit is disabled
        bool nonExtraCreditCondition = elevatorAtOurFloor && doorsOpen && !onElevator && !ENABLE_EXTRA_CREDIT;

        // If either the extra credit condition or the non-extra credit condition is true, then we can get on the elevator
        if (extraCreditCondition || nonExtraCreditCondition)
        {
            // If this is not the first move, we need to advance the itinerary index
            if (!firstMove)
            {
                advanceItinerary(thisPerson);
            }
            // Board the elevator
            boardElevator(getPersonKey(thisPerson));
            // Request the next floor
            requestFloor(nextToDoItem->floor);
            // Print that we're getting on the elevator
            printf("Person Number %d: Taking elevator to floor %d\n", getPersonKey(thisPerson), nextToDoItem->floor);
            // We're no longer on the first move anymore
            firstMove = false;
        }
        // If we're on the elevator and we're at our destination floor, then we can get off the elevator
        // This works because we update the itinerary index to the next item when we get on the elevator
        // -So if we're on the elevator and we're at our destination floor, then we know we're at the correct floor
        else if (elevatorAtOurFloor && doorsOpen && onElevator)
        {
            leaveElevator(getPersonKey(thisPerson));
            wanderedCurrentFloor = false;
            printf("Person Number %d: Got off at floor %d\n", getPersonKey(thisPerson), currentToDoItem->floor);
        }
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
    // Initialize the mutexes:
    initializeMutexes();

    // Initialize any global variables at the start:
    currentFloorGlobal = 0;

    // Initialize the directionMutex to 1:
    elevatorDirectionGlobal = ELEVATOR_GOING_UP;

    // Initialize the openDoorsMutex to closed:
    openDoorsGlobal = false;

    // Parse input parameters
    int *parsed = malloc(sizeof(int) * 3);
    parseParameters(parsed, argc, argv);

    // Parse parameters returns an array pointer, where 0 is the number of people, 1 is the wandering time, and 2 is the number of floors
    setPeopleCount(parsed[0]);
    int passengerCount = parsed[0];
    setWanderingTime(parsed[1]);
    int floorCount = parsed[2];

    // Free the parsed array
    free(parsed);

    // Initilize the elevatorRosterGlobal array to the size of the passengerCount
    elevatorRosterGlobal = malloc(sizeof(int) * passengerCount);
    for (int i = 0; i < passengerCount; i++)
    {
        elevatorRosterGlobal[i] = -1;
    }

    // Initialize the requestFloorGlobal array to the size of the floorCount
    requestFloorGlobal = malloc(sizeof(bool) * floorCount);
    for (int i = 0; i < floorCount; i++)
    {
        requestFloorGlobal[i] = false;
    }

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
    pthread_create(&elevatorThread, NULL, (void *(*)(void *))elevator, (void *)currentFloorGlobal);

    // Create a person thread for each Person:
    pthread_t personThreads[passengerCount];
    // The following code creates P-amount of threads:
    for (int personIndex = 0; personIndex < passengerCount; personIndex++)
    {
        // Get corresponding Person object to use inside person's thread:
        Person *thisPerson = people[personIndex];

        // Will create a thread:
        pthread_create(&personThreads[personIndex], NULL, (void *(*)(void *))person, (void *)thisPerson);
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