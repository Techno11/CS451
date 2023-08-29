/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 3
        Date of Submission: 8/29/2023
        Name of this file: elevatorTools.h
        Short Description of contents:
            This file contains the prototypes for the elevatorTools methods
*/

#ifndef ELEVATOR_TOOLS_H
#define ELEVATOR_TOOLS_H

#include <stdio.h>
#include "../structs/person.h"

/*======== SEE 'elevatorTools.c' FOR FUNCTION HEADER COMMENTS ========*/

void printPeople(Person *people[], int count);

void printWaitingAtFloor(int *waitingAtFloor, long floorCount);

#endif // ELEVATOR_TOOLS_H