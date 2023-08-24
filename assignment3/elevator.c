/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 3
        Date of Submission: 8/99/2023
        Name of this file: elevator.c
        Short Description of contents:
            blah blah blah
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "floor.c"
#include "person.c"
#include "itinerary.c"
#include "utils/setupHelper.c"
#include "utils/devtools.c"


int main(int argc, char *argv[])
{
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

    // Sanity Checks
    printPeople(people, passengerCount);
    printFloors(floors, floorCount);

    // Cleanup
    freeFloors(floors, floorCount);
    freePeople(people, passengerCount);
}