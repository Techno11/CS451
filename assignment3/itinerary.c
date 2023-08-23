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
    Struct: Floor

    Member variables:
        person - person to be initialized

    Brief description of the structure:
        A representation of the theorhetical floor
*/
Itinerary* initItineraryStruct(Itinerary* itinerary, int floor, int wanderTime)
{
    itinerary->floor = floor;
    itinerary->wanderTime = wanderTime;
    return itinerary;
}