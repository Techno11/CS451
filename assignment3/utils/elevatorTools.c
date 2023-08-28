/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 3
        Date of Submission: 8/99/2023
        Name of this file: devtools.c
        Short Description of contents:
            This file contains the elevatorTools methods, methods to help the elevator
*/

#include <stdio.h>
#include "elevatorTools.h"

void printPeople(Person *people[], int count)
{
    // Iterate through people
    for (int i = 0; i < count; i++)
    {
        // If person is null, skip
        if (people[i] == NULL)
            continue;

        // Print personand their floors to visit
        printf("Person %d: Floors To Visit ", i);
        for (int j = 0; j < people[i]->itinerarySize; j++)
        {
            // If last floor, don't print comma
            if (j == people[i]->itinerarySize - 1)
                printf("%d", people[i]->itinerary[j]->floor);
            else // Print comma
                printf("%d,", people[i]->itinerary[j]->floor);
        }
        printf("\n");

        // Print person and their time to spend
        printf("Person %d: Time To Spend ", i);
        for (int j = 0; j < people[i]->itinerarySize; j++)
        {
            // If last floor, don't print comma
            if (j == people[i]->itinerarySize - 1)
                printf("%d", people[i]->itinerary[j]->wanderTime);
            else // Print comma
                printf("%d,", people[i]->itinerary[j]->wanderTime);
        }
        printf("\n");
    }
}

void printWaitingAtFloor(int *waitingAtFloor, long floorCount)
{
    printf("Number of people waiting at floor:\n");
    printf("Floor Number Number of People\n");
    for (int i = 0; i < floorCount; i++)
    {
        printf("%-12d %d\n", i, waitingAtFloor[i]);
    }
}