/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 3
        Date of Submission: 8/29/2023
        Name of this file: setupHelper.h
        Short Description of contents:
            This file contains the methods that help during the application setup
*/

#include <stdio.h>
#include "setupHelper.h"


/*
    Function Name: parseParameters

    Input to Method:
        int *storage - An array of size 3 to store the parsed parameters
        int argc - The number of arguments (passed directly from the main method)
        char *argv[] - The arguments (passed directly from the main method)

    Output (Return value):
        N/A (void)

    Brief description of the task:
        Parse the command line arguments and store them in the storage array
*/
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

    // NOTE: THE PDF USES THE WRONG "-" ("–") so we check for both

    // Iterate over arguments and parse them
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "–p") == 0)
        {
            // Parse the next argument as an int
            passengerCount = atoi(argv[i + 1]);
            i++;
        }
        else if (strcmp(argv[i], "-w") == 0 || strcmp(argv[i], "–w") == 0)
        {
            wanderingTime = atoi(argv[i + 1]);
            i++;
        }
        else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "–f") == 0)
        {
            floorCount = atoi(argv[i + 1]);
            i++;
        }
    }

    storage[0] = passengerCount;
    storage[1] = wanderingTime;
    storage[2] = floorCount;
}

/*
    Function Name: readSetupStdin

    Input to Method:
        Person *people[] - An array of people to store the parsed passengers
        int floorCount - The number of floors
        int passengerCount - The number of passengers
        int maxWanderTime - The maximum wander time

    Output (Return value):
        N/A (void)

    Brief description of the task:
        Parse the standard input that contains the passenger's itineraries
*/
void readSetupStdin(Person *people[], int floorCount, int passengerCount, int maxWanderTime)
{
    // Allocate memory to People
    memset(people, 0, sizeof(Person *) * passengerCount);

    // Initilize all people to be null
    for (int i = 0; i < passengerCount; i++)
    {
        people[i] = NULL;
    }

    // Current passenger
    int currentPassenger = 0;

    // Parse lines from standard input. Each line is a passenger
    char *line = malloc(100 * sizeof(char));
    while (currentPassenger < passengerCount)
    {
        // Read line
        fgets(line, 100, stdin);

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

            // If the wander time is greater than the max, set it to the max
            if (people[currentPassenger]->itinerary[i]->wanderTime > maxWanderTime)
            {
                people[currentPassenger]->itinerary[i]->wanderTime = maxWanderTime;
            }
        }

        // Key their array ID:
        setPersonKey(people[currentPassenger], currentPassenger);

        // Increment current passenger
        currentPassenger++;
    }
    free(line);
}

/*
    Function Name: freePeople

    Input to Method:
        Person *people[] - An array of people to store the parsed passengers
        int passengerCount - The number of passengers

    Output (Return value):
        N/A (void)

    Brief description of the task:
        Free up the memory allocated to the people array
*/
void freePeople(Person *people[], int passengerCount)
{
    // Free up memory
    for (int i = 0; i < passengerCount; i++)
    {
        if (people[i] != NULL)
        {
            // if (people[i]->itinerary != NULL)
            //     free(people[i]->itinerary);
            free(people[i]);
        }
    }
}
