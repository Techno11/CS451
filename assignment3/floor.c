/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 3
        Date of Submission: 8/99/2023
        Name of this file: floor.c
        Short Description of contents:
            This file contains the person data structure
*/
#include "floor.h"
#include <stdlib.h>

/*
    Struct: Floor

    Member variables:
        person - person to be initialized

    Brief description of the structure:
        A representation of the theorhetical floor
*/
Floor* initFloorStruct(Floor* floor, int floorNumber, int floorSize)
{
    floor->floorNumber = floorNumber;
    floor->floorSize = floorSize;
    for(int i = 0; i < floorSize; i++)
    {
        floor->peopleOnFloor[i] = -1;
    }

    return floor;
}