/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 3
        Date of Submission: 8/99/2023
        Name of this file: floor.h
        Short Description of contents:
            This file contains the prototypes for the floor data structure
*/

#ifndef FLOOR_H
#define FLOOR_H

#include <stdio.h>
#include <sys/types.h>
#include <stdbool.h>

/*
    Struct: Floor

    Member variables:
        peopleOnFloor - Array of ints representing people on the floor
        floorNumber - The floor number that this is
        floorSize - max number of people that can be on this floor

    Brief description of the structure:
        A representation of the theorhetical person
*/
typedef struct Floor
{
    int floorNumber;
    int floorSize;
    int peopleOnFloor[];
} Floor;

/*======== SEE 'person.c' FOR FUNCTION HEADER COMMENTS ========*/
Floor* initFloorStruct(Floor* floor, int floorNumber, int floorSize);

#endif // FLOOR_H