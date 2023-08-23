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
#include "person.c"
#include "itinerary.c"

void printPeople(Person *people[], int count)
{
    for(int i = 0; i < count; i++)
    {
        if(people[i] == NULL)
            continue;
        printf("Person %d\n", i);
        printf("    Itinerary Size: %d\n", people[i]->itinerarySize);
        printf("    Current Itinerary Item: %d\n", people[i]->currentItenerayItem);
        for(int j = 0; j < people[i]->itinerarySize; j++)
        {
            printf("        Itinerary Item %d\n", j);
            printf("            Floor: %d\n", people[i]->itinerary[j]->floor);
            printf("            Wander Time: %d\n", people[i]->itinerary[j]->wanderTime);
        }
    }
}

void printFloors(int floors[][10], int floorCount, int passengerCount) {
    printf("Floors:\n");
    for (int i = 0; i < floorCount; i++)
    {
        printf("    Floor %d: ", i);
        for (int j = 0; j < passengerCount; j++)
        {
            printf("%d ", floors[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[])
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

    printf("Passenger Count: %d\n", passengerCount);
    printf("Wandering Time: %d\n", wanderingTime);
    printf("Floor Count: %d\n", floorCount);

    // Create our "Building"
    int floors[floorCount][passengerCount];
    memset( floors, 0, floorCount*passengerCount*sizeof(int) );

    // Initilize all floors to be null
    for (int i = 0; i < floorCount; i++)
    {
        for (int j = 0; j < passengerCount; j++)
        {
            floors[i][j] = -1;
        }
    }

    // Create our people lookup
    Person *people[passengerCount];
    memset( people, 0, sizeof(Person*) * passengerCount );

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
        people[currentPassenger] = malloc(sizeof(Person*) + (sizeof(Itinerary*) * numPairs));
        people[currentPassenger] = initPersonStruct(people[currentPassenger], numPairs);

        // Extract pairs. Each pair is an itinerary item
        for (int i = 0; i < numPairs; i++)
        {
            // Create itenaray item
            people[currentPassenger]->itinerary[i] = malloc(sizeof(Itinerary*));
            people[currentPassenger]->itinerary[i]->floor = atoi(strtok(NULL, " "));
            people[currentPassenger]->itinerary[i]->wanderTime = atoi(strtok(NULL, " "));
        }

        // Add person to floor
        floors[0][currentPassenger] = currentPassenger;

        // Increment current passenger
        currentPassenger++;

        // If we've reached the end of our passenger count, stop
        if(currentPassenger >= passengerCount)
            break;
    }

    printPeople(people, passengerCount);
    printFloors(floors, floorCount, passengerCount);
}