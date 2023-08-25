/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 3
        Date of Submission: 8/99/2023
        Name of this file: itinerary.h
        Short Description of contents:
            This file contains the prototypes for the itinerary data structure
*/

#ifndef ITENERARY_H
#define ITENERARY_H

#include <stdio.h>
#include <sys/types.h>
#include <stdbool.h>

/*
    Struct: Itinerary

    Member variables:
        itinerary - Array of itinerary objects for the person

    Brief description of the structure:
        A representation of the theorhetical person
*/
typedef struct Itinerary
{
    int floor;
    int wanderTime;
} Itinerary;

/*======== SEE 'person.c' FOR FUNCTION HEADER COMMENTS ========*/
Itinerary* initItineraryStruct(Itinerary* itinerary, int floor, int wanderTime);

#endif // ITENERARY_H