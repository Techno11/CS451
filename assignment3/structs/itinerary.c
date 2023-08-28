/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 3
        Date of Submission: 8/29/2023
        Name of this file: itinerary.c
        Short Description of contents:
            This file contains the itinerary data structure
*/
#include "person.h"

/*
    Function Name: initItineraryStruct

    Input to Method:
        Itinerary *itinerary - The itinerary to get the floor for
        int floor - The floor to set for the itinerary item
        int wanderTime - The wander time to set for the itinerary item

    Output (Return value):
        Itinerary* - The itinerary with the floor and wander time set

    Brief description of the task:
        Initilize an itinerary struct with a floor and wander time
*/
Itinerary* initItineraryStruct(Itinerary* itinerary, int floor, int wanderTime)
{
    itinerary->floor = floor;
    itinerary->wanderTime = wanderTime;
    return itinerary;
}

/*
    Function Name: getItineraryFloor

    Input to Method:
        Itinerary *itinerary - The itinerary to get the floor for

    Output (Return value):
        int - the floor of the itinerary item

    Brief description of the task:
        Get the floor of an itinerary item
*/
int getItineraryFloor(Itinerary* itinerary)
{
    int floorVal = itinerary->floor;
    return floorVal;
}