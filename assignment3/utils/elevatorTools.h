/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 3
        Date of Submission: 8/99/2023
        Name of this file: elevatorTools.h
        Short Description of contents:
            This file contains the prototypes for the elevatorTools methods
*/

#ifndef ELEVATOR_TOOLS_H
#define ELEVATOR_TOOLS_H

#include <stdio.h>
#include "../structs/floor.h"
#include "../structs/person.h"

/*======== SEE 'elevatorTools.c' FOR FUNCTION HEADER COMMENTS ========*/
void printFloors(Floor *floors[], int floorCount);

void printPeople(Person *people[], int count);

#endif // ELEVATOR_TOOLS_H