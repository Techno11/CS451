/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 3
        Date of Submission: 8/99/2023
        Name of this file: itinerary.c
        Short Description of contents:
            This file contains the itinerary data structure
*/
#include "person.h"
#include <stdlib.h>

/*
    Struct: Itinerary

    Member variables:
        floor - the floor to go to (?)
        wanderTime - the amount of time a person should idle on that floor (?)

    Brief description of the structure:
        A representation of the person's itinerary
*/
Itinerary* initItineraryStruct(Itinerary* itinerary, int floor, int wanderTime)
{
    itinerary->floor = floor;
    itinerary->wanderTime = wanderTime;
    return itinerary;
}