/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 3
        Date of Submission: 8/29/2023
        Name of this file: elevator.c
        Short Description of contents:
            Main part of elevator program, reads input and kicks off all child threads
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h> // for sleep()
#include "structs/person.c"
#include "structs/itinerary.c"
#include "utils/setupHelper.c"
#include "utils/elevatorTools.c"
#include "utils/mutexes.c" // Includes mutexes and global variables

// Macros
#define ELEVATOR_GOING_UP 1
#define ELEVATOR_GOING_DOWN 0

// Toggles the extra credit functionality
#define ENABLE_EXTRA_CREDIT false // true

/*
    Function Name: safetyNetForBadElevatorExit

    Input to Method:
        long lastFloorSeen - value that details what floor number the elevator just saw before exiting

    Output (Return value):
        N/A (void)

    Brief description of the task:
        A helper function that provides a sanity output check if elevator thread process dropped out badly.

*/
void safetyNetForBadElevatorExit(long lastFloorSeen)
{
    printf("                                                        Elevator: I am about to incorrectly stop, with a current floor of %ld.\n", lastFloorSeen);

    // ...

    /* This is one of the ways to exit a thread:
        -Here, it specifies an exit status value of 0
        which is available to another thread in the same
        process that called pthread_join().
            -AKA, just as a fun fact, the main() can be
            given the elevator's thread exit status.
    */
    pthread_exit(0);
}

/*
    Function Name: checkIfElevatorCanGoIdle

    Input to Method:
        int numOfEmptyFloorsSeen - empty floors counter to initially tell how many people are not waiting
        (then use the empty floors count to iterate through and see how many floors are literally empty)
        long maxAmountOfFloors - the max amount of floors in the building
        long currentFloor - used for passing any current floor global variable to the elevator thread

    Output (Return value):
        N/A (void)

    Brief description of the task:
        A helper function that communicates if the elevator can sleep for the time being, and perhaps exit.

*/
void checkIfElevatorCanGoIdle(int numOfEmptyFloorsSeen, long maxAmountOfFloors, long currentFloor)
{
    if ((numOfEmptyFloorsSeen >= (maxAmountOfFloors * 2)) && (currentFloor == 0)) { //emptyPassSets = 2 * N floors traveled by elevator
        printf("Waiting for max waiting time. No one is waiting for the elevator.\n");
        sleep(getWanderingTime()); // get max wander time for elevator sleepy-sleep
        int *waitingArray = getWaitingAtAllFloors();
        int totalWaitingCount = 0;
        for (int floorInd = 0; floorInd < maxAmountOfFloors; floorInd++) {
            totalWaitingCount += waitingArray[floorInd];
        }
        if (totalWaitingCount == 0) {
            printf("Elevator Leaving The System.\n");
            pthread_exit(0);
        }
    }
}

/*
    Function Name: hasElevatorReachedTheBottom

    Input to Method:
        int* elevatorDirection - directional vector value of this elevator which is compared to macros
        long maxAmountOfFloors - the max amount of floors in the building
        long currentFloor - used for passing any current floor global variable to the elevator thread

    Output (Return value):
        bool - true/false describing if this elevator is at the bottom floor or not

    Brief description of the task:
        A helper function that communicates with true/false return if elevator is at the bottom floor or not.

*/
bool hasElevatorReachedTheBottom(int *elevatorDirection, long maxAmountOfFloors, long currentFloor)
{
    // If we're at the min floor, change direction and print
    if (currentFloor == 0)
    {
        // Print where we're at
        printWaitingAtFloor(getWaitingAtAllFloors(), maxAmountOfFloors + 1);
        printf("                                                        Elevator: At floor %ld.\n", currentFloor);
        printf("                                                        Elevator: Heading to max Floor %ld\n", maxAmountOfFloors);
        setElevatorDirectionGlobal(ELEVATOR_GOING_UP);
        *elevatorDirection = ELEVATOR_GOING_UP;

        return true;
    }
    else {
        return false;
    }
}

/*
    Function Name: hasElevatorReachedTheTop

    Input to Method:
        bool isThisAtTheBottomFloor - true/false that communicates an if-elseif structure with top vs. bottom
        int* elevatorDirection - directional vector value of this elevator which is compared to macros
        long maxAmountOfFloors - the max amount of floors in the building
        long currentFloor - used for passing any current floor global variable to the elevator thread

    Output (Return value):
        bool - true/false describing if this elevator has reached the top floor or not

    Brief description of the task:
        A helper function that communicates with a true/false return if elevator is at the top floor or not.

*/
bool hasElevatorReachedTheTop(bool isThisAtTheBottomFloor, int *elevatorDirection, long maxAmountOfFloors,
                              long currentFloor)
{
    // If we're at the max floor, change direction and print
    if (!isThisAtTheBottomFloor && (currentFloor == maxAmountOfFloors))
    {
        // Print where we're at
        printWaitingAtFloor(getWaitingAtAllFloors(), maxAmountOfFloors + 1);
        printf("                                                        Elevator: At floor %ld.\n", currentFloor);
        printf("                                                        Elevator: Heading to min Floor %d\n", 0);
        setElevatorDirectionGlobal(ELEVATOR_GOING_DOWN);
        *elevatorDirection = ELEVATOR_GOING_DOWN;

        return true;
    }
    else {
        return false;
    }
}

/*
    Function Name: isCallButtonPushedOnThisFloor

    Input to Method:
        int *numOfEmptyFloorsSeen - empty floors counter to be updated if no one called to this floor
        long currentFloor - used for passing any current floor global variable to the elevator thread

    Output (Return value):
        N/A (void)

    Brief description of the task:
        A helper function that checks if this elevator has been called to this current floor.

*/
void isCallButtonPushedOnThisFloor(int *numOfEmptyFloorsSeen, long currentFloor)
{
    // If someone is waiting on this floor, we need to open the doors
    if (isFloorRequested(currentFloor))
    {
        *numOfEmptyFloorsSeen = 0 + 1; // reset back to starting Floor 1 (0th floor)

        // Open the doors
        setDoorsOpen(true);
        // Print that we've stopped
        printf("                                                        Elevator: Opening the doors at %ld\n", currentFloor);
        // Wait for people to get on
        sleep(1);
        // Close the doors
        setDoorsOpen(false);
        clearFloorRequest(currentFloor);

    }
    else
    {
        *numOfEmptyFloorsSeen = *numOfEmptyFloorsSeen + 1;
    }
}

/*
    Function Name: hoistOrLowerThisElevator

    Input to Method:
        int elevatorDirection - directional vector value of this elevator which is compared to macros
        long* currentFloor - used for passing and updating any current floor global variable to the elevator thread

    Output (Return value):
        N/A (void)

    Brief description of the task:
        A helper function that simply raises or lowers this elevator by one floor.

*/
void hoistOrLowerThisElevator(int elevatorDirection, long *currentFloor)
{
    // Go up a floor if the established direction is up:
    if (elevatorDirection == ELEVATOR_GOING_UP)
    {
        *currentFloor = *currentFloor + 1;
        setCurrentFloor(*currentFloor);
    }
    // Else, go down a floor (if the established direction is down):
    else
    {
        *currentFloor = *currentFloor - 1;
        setCurrentFloor(*currentFloor);
    }
}

/*
    Function Name: elevator

    Input to Method:
        long currentFloor - a dedicated spot for passing any current floor global variable to the elevator thread

    Output (Return value):
        N/A (void)

    Brief description of the task:
        A thread handler method for coordinating the synchronization of an elevator process thread.

*/
void elevator(long currentFloor)
{
    // Variable to track things locally, so we don't have to read them from the global variables every time:
    long currentFloorLocal = getCurrentFloor();
    int elevatorDirectionGlobalLocal = getElevatorDirection();

    // The "currentFloor" currently says the top floor, so we need to record that and then reset it to 0:
    long maxFloor = currentFloorLocal;

    // Set the current floor to 0:
    setCurrentFloor(0);
    currentFloorLocal = 0;

    // Wait for people to enter the building
    sleep(1);

    int emptyPassSets = 0 + 1; // To offset for floor 1 to N (instead of 0 to N-1)

    while (true)
    {
        // See if the elevator can sleep for maxWanderTime and possibly exit out:
        checkIfElevatorCanGoIdle(emptyPassSets, maxFloor, currentFloorLocal);

        // See if this elevator is at the bottom floor, and if so, change direction and print:
        bool isAtTheBottomFloor = hasElevatorReachedTheBottom(&elevatorDirectionGlobalLocal, maxFloor, currentFloorLocal);

        // See if this elevator is at the top floor, and if so, change direction and print:
        bool isAtTheTopFloor = hasElevatorReachedTheTop(isAtTheBottomFloor, &elevatorDirectionGlobalLocal, maxFloor, currentFloorLocal);

        // Check if someone called the elevator from this floor:
        isCallButtonPushedOnThisFloor(&emptyPassSets, currentFloorLocal);

        // Transition between floors
        sleep(1);

        // Go up (or down) a floor depending on this elevator's direction:
        hoistOrLowerThisElevator(elevatorDirectionGlobalLocal, &currentFloorLocal);
    }

    safetyNetForBadElevatorExit(currentFloor);
}

/*
    Function Name: getWhatFloorImOnAndWhereToNext

    Input to Method:
        Person* myself - the keyed Person object associated with this Person thread instance.
        Itinerary** currentToDo - the item on the Person's list describing where we are right now and what
                                 we are doing which will be updated
        Itinerary** nextToDoItem - the item on the Person's list describing where we go next and what we do there
        int* findThisFloor - find and update the current floor this Person is on, represented by an index
        bool justEntered - true/false detailing if this is the first time the Person has entered the building

    Output (Return value):
        N/A (void)

    Brief description of the task:
        A helper function for coordinating where the Person is in the building and where it needs to go.
*/
void getWhatFloorImOnAndWhereToNext(Person *myself, Itinerary **currentToDo,
                                    Itinerary **nextToDo, int *findThisFloor, bool justEntered)
{
    // Fetch the current itinerary item and the next itinerary item:
    *currentToDo = getCurrentItineraryItem(myself);
    *nextToDo = getNextItineraryItem(myself);

    // Get the current floor we're on. If this is our first move, then we're on floor 0
    //findThisFloor = currentToDo->floor;
    *findThisFloor = getItineraryFloor(*currentToDo);
    if (justEntered)
    {
        *findThisFloor = 0;
        *nextToDo = *currentToDo;
    }
}

/*
    Function Name: startWanderingAround

    Input to Method:
        Person* myself - the keyed Person object associated with this Person thread instance.
        bool* didIWanderThisFloor - true/false to be updated to reflect if floor has been wandered by this Person
        bool justEntered - true/false detailing if this is the first time the Person has entered the building
        Itinerary* currentToDoItem - the item on the Person's list describing where we are right now and what we are doing
        Itinerary* nextToDoItem - the item on the Person's list describing where we go next and what we do there
        int theFloorWeAreOn - index detailing the floor number that this Person is on

    Output (Return value):
        N/A (void)

    Brief description of the task:
        A helper function for coordinating and making the Person thread wander around a floor.
*/
bool startWanderingAround(Person *myself, bool *didIWanderThisFloor, bool justEntered,
                          Itinerary *currentToDoItem, Itinerary *nextToDoItem, int theFloorWeAreOn)
{
    bool isThereNoMoreWanderingLeft = false;
    bool hasFloorBeenWandered = *didIWanderThisFloor;

    // If we haven't wandered the current floor, then wander it:
    if (!hasFloorBeenWandered)
    {
        if (!justEntered)
        {
            // Get, print, and wait for wandering time:
            int wanderTime = currentToDoItem->wanderTime;
            printf("Person Number %d: Wandering for %d seconds\n", getPersonKey(myself), wanderTime);
            sleep(wanderTime);

            // Set that we're waiting at this floor
            incrementWaitingCount(theFloorWeAreOn);
        }

        // We've wandered this floor
        *didIWanderThisFloor = true;

        // If next item is null, then we're done
        if (nextToDoItem == NULL)
        {
            isThereNoMoreWanderingLeft= true; //break;
        }

        if (!isThereNoMoreWanderingLeft) {
            // Print that we're waiting for the elevator
            printf("Person Number %d: Waiting for elevator at floor %d\n", getPersonKey(myself), theFloorWeAreOn);
        }
    }
    return isThereNoMoreWanderingLeft;
}

/*
    Function Name: updateGetOnElevatorConditions

    Input to Method:
        bool* extraCreditCondition - an evaluated condition for elevator entry that factors in direction
        bool* nonExtraCreditCondition - an evaluated condition for normal elevator entry
        bool* elevatorAtOurFloor - an updated true/false describing pre req. that the elevator is on the same floor
        bool* doorsOpen - an updated true/false describing pre req. that the elevator's doors are open for us
        bool* onElevator - an updated true/false describing pre req. that we haven't been on the elevator already
        bool elevatorUpAndWeNeedToGoUp - true/false to say if our directions are aligned upwards or not
        bool elevatorDownAndWeNeedToGoDown - true/false to say if our directions are aligned downwards or not

    Output (Return value):
        N/A (void)

    Brief description of the task:
        A helper function to update whether the Person can (or even should) get on the elevator.
*/
void updateGetOnElevatorConditions(bool *extraCreditCondition, bool *nonExtraCreditCondition,
                                   bool *elevatorAtOurFloor, bool *doorsOpen, bool *onElevator,
                                   bool elevatorUpAndWeNeedToGoUp, bool elevatorDownAndWeNeedToGoDown)
{
    // If:
    // -the elevator is at our floor,
    // -the doors are open,
    // -we're not already on the elevator,
    // -the elevator is going the correct direction,
    // -and extra credit is enabled, then determine the extra credit condition!
    *extraCreditCondition = *elevatorAtOurFloor && *doorsOpen && !*onElevator &&
                            (elevatorUpAndWeNeedToGoUp || elevatorDownAndWeNeedToGoDown) && ENABLE_EXTRA_CREDIT;

    // If:
    // -the elevator is at our floor,
    // -the doors are open,
    // -we're not already on the elevator,
    // -and extra credit is disabled, then just determine the condition normally:
    *nonExtraCreditCondition = *elevatorAtOurFloor && *doorsOpen && !*onElevator && !ENABLE_EXTRA_CREDIT;
}

/*
    Function Name: decideWhetherToBecomeAwareOfElevatorDirection

    Input to Method:
        Person* forThisPerson - the keyed Person object associated with this Person thread instance.
        bool* elevatorAtOurFloor - true/false to be updated as to reflect if elevator is at Person's floor
        bool* doorsOpen - true/false to be updated as to reflect if elevator shaft doors are open access
        bool* onElevator - true/false to be updated as to reflect if this Person is on the elevator
        int theFloorWeAreOn - index detailing the floor number that this Person is on
        Itinerary* nextToDoItem - the item on the Person's list describing where we go next and what we do there
        int theMaxFloor - index representing the max floor level in the building
        bool* extraCreditCondition - an evaluated condition for elevator entry that factors in direction
        bool* nonExtraCreditCondition - an evaluated condition for normal elevator entry

    Output (Return value):
        N/A (void)

    Brief description of the task:
        A helper function for deciding if the Person should care about elevator's direction when tracking.
*/
void decideWhetherToBecomeAwareOfElevatorDirection(Person *forThisPerson, bool *elevatorAtOurFloor, bool *doorsOpen,
                                                   bool *onElevator, int theFloorWeAreOn, Itinerary *nextToDoItem,
                                                   int theMaxFloor, bool *extraCreditCondition,
                                                   bool *nonExtraCreditCondition)
{
    // If you are done waiting && you are going up && elevator is going up then "get on" && the doors are open
    //  -Use defined constants (#'s) at the top
    //  -Current itinerary item is where the Person is AT
    //  -NEXT itinerary item is where the Person is GOING (destination floor)
    long currentFloor = getCurrentFloor();
    int elevatorDirection = getElevatorDirection();

    // Calculate keys to getting on/off the elevator
    /***  --Necessary Info To Update-- ***/
    *elevatorAtOurFloor = currentFloor == theFloorWeAreOn;                           // Is the elevator at our floor?
    *doorsOpen = getDoorsOpen();                                                     // Are the doors open?
    *onElevator = getIsOnElevator(getPersonKey(forThisPerson));                         // Are we on the elevator?
    /***  --Extra Vector-like Info To Consider If Becoming Aware-- ***/
    bool doWeNeedToGoUp = nextToDoItem != NULL && nextToDoItem->floor > theFloorWeAreOn; // Do we need to go up
    bool elevatorGoingUp = elevatorDirection == ELEVATOR_GOING_UP;                       // Is the elevator going up?
    bool elevatorUpAndWeNeedToGoUp = elevatorGoingUp && doWeNeedToGoUp;                  // Is the elevator going up and we need to go up?
    bool elevatorDownAndWeNeedToGoDown = !elevatorGoingUp && !doWeNeedToGoUp;            // Is the elevator going down and we need to go down?

    // Request elevator to stop at our floor if it's going the correct direction and hasn't passed, and extra credit is enabled
    if ((elevatorUpAndWeNeedToGoUp || elevatorDownAndWeNeedToGoDown) && !*onElevator && ENABLE_EXTRA_CREDIT)
    {
        // Don't request the elevator if it's already passed our floor
        bool passedGoingUp = elevatorGoingUp && currentFloor > theFloorWeAreOn;         // Has the elevator passed our floor going up?
        bool passedGoingDown = !elevatorGoingUp && currentFloor < theFloorWeAreOn;      // Has the elevator passed our floor going down?
        bool doorsClosedAtOurFloor = !*doorsOpen && currentFloor == theFloorWeAreOn;     // Are the doors closed at our floor?
        bool topFloorException = theFloorWeAreOn == theMaxFloor - 1;                       // Are we on the top floor?
        if ((!passedGoingUp && !passedGoingDown && !doorsClosedAtOurFloor) || topFloorException)
        {
            requestFloor(theFloorWeAreOn);
        }
    }
        // Request elevator to stop at our floor regardless of direction if extra credit is disabled
    else if (!*onElevator && !ENABLE_EXTRA_CREDIT)
    {
        requestFloor(theFloorWeAreOn);
    }

    // Evaluate if we can get on the elevator, so we can decide if we should later:
    updateGetOnElevatorConditions(extraCreditCondition, nonExtraCreditCondition, elevatorAtOurFloor,
                                  doorsOpen, onElevator, elevatorUpAndWeNeedToGoUp,
                                  elevatorDownAndWeNeedToGoDown);
}

/*
    Function Name: shouldIGetOnOrOffTheElevator

    Input to Method:
        Person* myself - the keyed Person object associated with this Person thread instance.
        bool* justEntered - true/false describing if this is the first time the Person has moved into building
        int theFloorWeAreOn - index detailing the floor number that this Person is on
        Itinerary* currentToDoItem - the item on the Person's list describing where we are right now and what we are doing
        Itinerary* nextToDoItem - the item on the Person's list describing where we go next and what we do there
        bool* wanderedCurrentFloor - true/false that updates as to whether this Person has wandered the floor fully
        bool extraCreditCondition - an evaluated condition for elevator entry that factors in direction
        bool nonExtraCreditCondition - an evaluated condition for normal elevator entry
        bool isElevatorAtOurFloor - true/false detailing if the elevator is at this Person's level
        bool areDoorsOpen - true/false detailing if the elevator shaft's doors are open or closed
        bool amIAlreadyOnTheElevator - true/false used for determining if Person should get on or off

    Output (Return value):
        N/A (void)

    Brief description of the task:
        A helper function for determining if a Person process thread should embark or disembark from
        the elevator process object.
*/
void shouldIGetOnOrOffTheElevator(Person *myself, bool *justEntered, int theFloorWeAreOn,
                                  Itinerary *currentToDoItem, Itinerary *nextToDoItem,
                                  bool *wanderedCurrentFloor, bool extraCreditCondition, bool nonExtraCreditCondition,
                                  bool isElevatorAtOurFloor, bool areDoorsOpen, bool amIAlreadyOnTheElevator)
{
    // If either the extra credit condition or the non-extra credit condition is true, then we can get on the elevator
    if (extraCreditCondition || nonExtraCreditCondition)
    {
        // If this is not the first move, we need to advance the itinerary index
        if (!*justEntered)
        {
            advanceItinerary(myself);
        }
        // Board the elevator
        boardElevator(getPersonKey(myself));
        // Decrement waiting at floor count
        decrementWaitingCount(theFloorWeAreOn);
        // Request the next floor
        requestFloor(nextToDoItem->floor);
        // Print that we're getting on the elevator
        printf("Person Number %d: Taking elevator to floor %d\n", getPersonKey(myself), nextToDoItem->floor);
        // We're no longer on the first move anymore
        *justEntered = false;
    }

        // If we're on the elevator, and we're at our destination floor, then we can get off the elevator
        // This works because we update the itinerary index to the next item when we get on the elevator
        // -So if we're on the elevator, and we're at our destination floor, then we know we're at the correct floor
    else if (isElevatorAtOurFloor && areDoorsOpen && amIAlreadyOnTheElevator)
    {
        leaveElevator(getPersonKey(myself));
        *wanderedCurrentFloor = false;
        printf("Person Number %d: Got off at floor %d\n", getPersonKey(myself), currentToDoItem->floor);
    }
}

/*
    Function Name: person

    Input to Method:
        Person* thisPerson - the keyed Person object associated with this Person thread instance.

    Output (Return value):
        N/A (void)

    Brief description of the task:
        A thread handler method for coordinating the synchronization of a Person process thread.
*/
void person(Person *thisPerson)
{
    // The "notDone" can be a method to check that Person's itinerary is not completed
    // Get max floor
    int maxFloor = getNumFloors();

    // Track whether or not we've wandered the current floor or itinerary item
    bool wanderedCurrentFloor = false;

    // Track whether this is our first move, and we just entered the building
    bool firstMove = true;

    // While the person's itinerary is not done, keep looping:
    while (!isItineraryDone(thisPerson))
    {
        // The current itinerary item and the next itinerary item which are to be obtained:
        Itinerary *currentToDoItem;
        Itinerary *nextToDoItem;

        // The current floor we're on, initially unknown (must find out)
        int theFloorWeAreOn;

        getWhatFloorImOnAndWhereToNext(thisPerson, &currentToDoItem,
                                       &nextToDoItem, &theFloorWeAreOn, firstMove);

        // This Person's itinerary is done if there's no more wandering around to do:
        bool isMyItineraryDone = startWanderingAround(thisPerson,
                                                      &wanderedCurrentFloor,firstMove,
                                                      currentToDoItem, nextToDoItem, theFloorWeAreOn);

        // Have person leave waiting count if their itinerary is done:
        if (isMyItineraryDone) {
            decrementWaitingCount(theFloorWeAreOn);
            break;
        }

        // Observe checks to see if elevator is available for us to ride, and if we should:
        bool elevatorAtOurFloor;
        bool doorsOpen;
        bool onElevator;
        bool extraCreditCondition;
        bool nonExtraCreditCondition;
        decideWhetherToBecomeAwareOfElevatorDirection(thisPerson, &elevatorAtOurFloor, &doorsOpen,
                                                      &onElevator, theFloorWeAreOn, nextToDoItem,
                                                      maxFloor, &extraCreditCondition,
                                                      &nonExtraCreditCondition);

        // Based on the state of the elevator and our state in relation to the elevator,
        // determine if we should get on or off the elevator:
        shouldIGetOnOrOffTheElevator(thisPerson, &firstMove, theFloorWeAreOn, currentToDoItem,
                                     nextToDoItem, &wanderedCurrentFloor, extraCreditCondition,
                                     nonExtraCreditCondition, elevatorAtOurFloor, doorsOpen,
                                     onElevator);
    }
    // When done print goodbye message and exit thread:
    printf("Person Number %d: Leaving the system, goodbye!\n", getPersonKey(thisPerson));
    pthread_exit(0);
}

void initializeGlobalArrayElementsMemory(int passengerCount, int floorCount)
{
    // Initialize the elevatorRosterGlobal array to the size of the passengerCount
    elevatorRosterGlobal = malloc(sizeof(int) * passengerCount);
    for (int i = 0; i < passengerCount; i++)
    {
        // Initially set starting values in the elevator's holding array to -1 to signify emptiness:
        elevatorRosterGlobal[i] = -1;
    }

    // Initialize the requestFloorGlobal array to the size of the floorCount
    requestFloorGlobal = malloc(sizeof(bool) * floorCount);
    // Elevator starts at beginning of day with doors open on BOTTOM floor (of "building")
    requestFloorGlobal[0] = true;
    for (int i = 1; i < floorCount; i++)
    {
        // All other floor doors are to be shut and their "call buttons" de-pressed:
        requestFloorGlobal[i] = false;
    }

    /* Initialize the waitingAtFloorGlobal array to the size of the floorCount,
       and set floor 0 to have passengerCount waiting:
    */
    waitingAtFloorGlobal = malloc(sizeof(int) * floorCount);
    waitingAtFloorGlobal[0] = passengerCount;
    for (int i = 1; i < floorCount; i++)
    {
        // At the start, there should no passengers waiting in the above floors since they all start at 0th floor:
        waitingAtFloorGlobal[i] = 0;
    }
}

void initializeGlobalMemory(int argc, char *argv[], int *passengerCount, int *floorCount)
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
    parseParameters(parsed, argc, argv); // < convert and organize the parameters into parsed array

    /* Parse parameters returns an array pointer, where inside parsed[]:
     * 0th element is the number of people,
     * 1st element is the wandering time,
     * and 2nd element is the number of floors.
    */
    setPeopleCount(parsed[0]);
    *passengerCount = parsed[0];
    setWanderingTime(parsed[1]);
    *floorCount = parsed[2];
    setNumFloors(parsed[2]);

    // Free the parsed array
    free(parsed);

    // Fill in all global arrays' elements with starting memory values:
    initializeGlobalArrayElementsMemory(*passengerCount, *floorCount);
}

/*
    Function Name: main

    Input to Method:
        int argc - the count/number of arguments entered from the command line
        char* argv[] - the array holding character-array strings where each argument from CMD is an array element

    Output (Return value):
        An integer representing successful completion of the main method.

    Brief description of the task:
        The main method that gets/starts everything going for the synchronization of created threads.

*/
int main(int argc, char *argv[])
{
    /***** BEGIN INITIALIZATION OF GLOBAL VARIABLES AND MEMORY *****/
    int passengerCount;
    int floorCount;

    initializeGlobalMemory(argc, argv, &passengerCount, &floorCount);

    // Create our people lookup
    Person *people[passengerCount];

    // Read standard input
    readSetupStdin(people, floorCount, passengerCount, getWanderingTime());

    // First thing program does is print input from stdin
    printPeople(people, passengerCount);

    // Set the current floor (global) to the max floor (which is N length - 1).
    //  -The elevator pthread then knows what the max floor is, and will reset it to 0
    setCurrentFloor(floorCount - 1);

    /***** GLOBAL INITIALIZATION COMPLETE *****/

    /***** BEGIN THE CREATION OF ELEVATOR AND PEOPLE THREADS  *****/

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

    /***** INITIALIZATION OF ELEVATOR AND PEOPLE THREADS COMPLETE *****/

    /***** BEGIN THE JOINING OF ELEVATOR AND PEOPLE THREADS TO THE MAIN THREAD FOR SYNCHRONIZATION RUNTIME *****/

    // Join the threads together with the calling main thread so that they have to wait for each other:
    pthread_join(elevatorThread, NULL);
    for (int personIndex = 0; personIndex < passengerCount; personIndex++)
    {
        pthread_join(personThreads[personIndex], NULL);
    }

    /***** ALL PEOPLE AND ELEVATOR THREADS NOW EXITED MAIN THREAD SYSTEM - THREADS COMPLETED *****/

    // Cleanup
    freePeople(people, passengerCount);

    return 0;
}