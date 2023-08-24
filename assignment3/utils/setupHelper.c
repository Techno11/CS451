/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 3
        Date of Submission: 8/99/2023
        Name of this file: setupHelper.h
        Short Description of contents:
            This file contains the prototypes for the setupHelpers methods
*/

#include <stdio.h>
#include "setupHelper.h"

void parseParameters(int *storage, int argc, char *argv[])
{
    // Possible Arguments to read in:
    // -p <int> : number of passengers
    // -w <int> : wandering time
    // -f <int> : floor count
    // The remaining arguments are from a file, following the given format:
    //      n f1 t1 f2 t2 f3 t3 ... fn tn

    // Default Arguments
    int passengerCount = 1;
    int wanderingTime = 10;
    int floorCount = 10;

    // Iterate over arguments and parse them
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-p") == 0)
        {
            // Parse the next argument as an int
            passengerCount = atoi(argv[i + 1]);
            i++;
        }
        else if (strcmp(argv[i], "-w") == 0)
        {
            wanderingTime = atoi(argv[i + 1]);
            i++;
        }
        else if (strcmp(argv[i], "-f") == 0)
        {
            floorCount = atoi(argv[i + 1]);
            i++;
        }
    }

    storage[0] = passengerCount;
    storage[1] = wanderingTime;
    storage[2] = floorCount;
}

void readSetupStdin(Floor *floors[], Person *people[], int floorCount, int passengerCount)
{
    // Allocate memory to Floors and People
    memset(floors, 0, sizeof(Floor) * floorCount);
    memset(people, 0, sizeof(Person *) * passengerCount);

    // Initilize all floors
    for (int i = 0; i < floorCount; i++)
    {
        floors[i] = malloc(sizeof(Floor *) + (sizeof(int) * passengerCount));
        floors[i] = initFloorStruct(floors[i], i, passengerCount);
    }

    // Initilize all people to be null
    for (int i = 0; i < passengerCount; i++)
    {
        people[i] = NULL;
    }

    // Current passenger
    int currentPassenger = 0;

    // Parse lines from standard input. Each line is a passenger
    char *line = malloc(100 * sizeof(char));
    while (fgets(line, 100, stdin) != NULL)
    {
        // Extract 1st number, which is the number of pairs to follow
        int numPairs = atoi(strtok(line, " "));

        // Create our passenger
        people[currentPassenger] = malloc(sizeof(Person *) + (sizeof(Itinerary *) * numPairs));
        people[currentPassenger] = initPersonStruct(people[currentPassenger], numPairs);

        // Extract pairs. Each pair is an itinerary item
        for (int i = 0; i < numPairs; i++)
        {
            // Create itenaray item
            people[currentPassenger]->itinerary[i] = malloc(sizeof(Itinerary *));
            people[currentPassenger]->itinerary[i]->floor = atoi(strtok(NULL, " "));
            people[currentPassenger]->itinerary[i]->wanderTime = atoi(strtok(NULL, " "));
        }

        // Add person to floor
        floors[0]->peopleOnFloor[currentPassenger] = currentPassenger;

        // Increment current passenger
        currentPassenger++;

        // If we've reached the end of our passenger count, stop
        if (currentPassenger >= passengerCount)
            break;
    }
    free(line);
}

void freePeople(Person *people[], int passengerCount)
{
    // Free up memory
    for (int i = 0; i < passengerCount; i++)
    {
        if (people[i] != NULL) {
            // if (people[i]->itinerary != NULL)
            //     free(people[i]->itinerary);
            free(people[i]);
        }
    }
}

void freeFloors(Floor *floors[], int floorCount)
{
    // Free up memory
    for (int i = 0; i < floorCount; i++)
    {
        if (floors[i] != NULL)
            free(floors[i]);
    }
}