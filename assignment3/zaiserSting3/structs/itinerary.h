/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 3
        Date of Submission: 8/29/2023
        Name of this file: itinerary.h
        Short Description of contents:
            This file contains the prototypes for the itinerary data structure
*/

#ifndef ITINERARY_H
#define ITINERARY_H

#include <stdio.h>
#include <sys/types.h>
#include <stdbool.h>

/*
    Struct: Itinerary

    Member variables:
        itinerary - Array of itinerary objects for the person

    Brief description of the structure:
        A representation of the theoretical person
*/
typedef struct Itinerary
{
    int floor;
    int wanderTime;
} Itinerary;

/*======== SEE 'person.c' FOR FUNCTION HEADER COMMENTS ========*/
Itinerary* initItineraryStruct(Itinerary* itinerary, int floor, int wanderTime);
int getItineraryFloor(Itinerary* itinerary);

#endif // ITINERARY_H