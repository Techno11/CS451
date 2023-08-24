/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 3
        Date of Submission: 8/99/2023
        Name of this file: devtools.h
        Short Description of contents:
            This file contains the prototypes for the devtools methods
*/

#ifndef DEVTOOLS_H
#define DEVTOOLS_H

#include <stdio.h>
#include "../structs/floor.h"
#include "../structs/person.h"

/*======== SEE 'setupHelper.c' FOR FUNCTION HEADER COMMENTS ========*/
void printFloors(Floor *floors[], int floorCount);

void printPeople(Person *people[], int count);

#endif // DEVTOOLS_H