/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 3
        Date of Submission: 8/99/2023
        Name of this file: setupHelper.h
        Short Description of contents:
            This file contains the prototypes for the setupHelpers methods
*/

#ifndef SETUP_HELPER_H
#define SETUP_HELPER_H

#include <stdio.h>
#include "../floor.h"
#include "../person.h"

/*======== SEE 'setupHelper.c' FOR FUNCTION HEADER COMMENTS ========*/
void parseParameters(int* storage, int argc, char *argv[]);

void readSetupStdin(Floor *floors[], Person *people[], int floorCount, int passengerCount);

void freePeople(Person *people[], int passengerCount);

void freeFloors(Floor *floors[], int floorCount);

#endif // SETUP_HELPER_H