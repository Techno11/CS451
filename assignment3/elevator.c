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

    // Parse lines from standard input
    char *line = malloc(100 * sizeof(char));
    while (fgets(line, 100, stdin) != NULL)
    {
        // Extract 1st number, which is the number of pairs to follow
        int numPairs = atoi(strtok(line, " "));

        // Extract pairs
        for (int i = 0; i < numPairs; i++)
        {
            // Extract floor
            int floor = atoi(strtok(NULL, " "));

            // Extract time
            int time = atoi(strtok(NULL, " "));

            printf("Floor: %d, Time: %d\n", floor, time);
        }
    }
}