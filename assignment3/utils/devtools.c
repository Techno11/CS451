/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 3
        Date of Submission: 8/99/2023
        Name of this file: devtools.c
        Short Description of contents:
            This file contains the devtools methods, methods to aid in development
*/

#include <stdio.h>
#include "devtools.h"

void printPeople(Person *people[], int count)
{
    for (int i = 0; i < count; i++)
    {
        if (people[i] == NULL)
            continue;
        printf("Person %d\n", i);
        printf("    Itinerary Size: %d\n", people[i]->itinerarySize);
        printf("    Current Itinerary Item: %d\n", people[i]->currentItenerayItem);
        for (int j = 0; j < people[i]->itinerarySize; j++)
        {
            printf("        Itinerary Item %d\n", j);
            printf("            Floor: %d\n", people[i]->itinerary[j]->floor);
            printf("            Wander Time: %d\n", people[i]->itinerary[j]->wanderTime);
        }
    }
}

void printFloors(Floor *floors[], int floorCount)
{
    printf("Floors:\n");
    for (int i = 0; i < floorCount; i++)
    {
        printf("    Floor %d: ", i);
        for (int j = 0; j < floors[i]->floorSize; j++)
        {
            printf("%d ", floors[i]->peopleOnFloor[j]);
        }
        printf("\n");
    }
}