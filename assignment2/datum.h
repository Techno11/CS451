/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 2
        Date of Submission: 7/24/2023
        Name of this file: datum.h
        Short Description of contents:
            This file contains the prototypes for the datum data structure
*/

#ifndef DATUM_H
#define DATUM_H

#include <stdio.h>
#include <sys/types.h>
#include <stdbool.h>

typedef struct Datum
{
    pid_t inputPID;
    //unsigned long inputBurst;
    time_t inputBurst;
    pid_t childPID;
} Datum;


Datum* initDatumStruct(Datum* datum, pid_t pid, unsigned long burst);

#endif