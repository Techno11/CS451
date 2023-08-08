/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 2
        Date of Submission: 8/9/2023
        Name of this file: datum.h
        Short Description of contents:
            This file contains the prototypes for the datum data structure
*/

#ifndef DATUM_H
#define DATUM_H

#include <stdio.h>
#include <sys/types.h>
#include <stdbool.h>

/*
    Struct: Datum

    Member variables:
        inputPID - the process ID of a Datum item as listed in the input file (e.g., input.txt)
        inputBurst - the burst time of a Datum item as listed on a line in the input file (e.g., input.txt)
        childPID - the actual child process ID after forking a line item in the input file

    Brief description of the structure:
        A compressed data object, that has key process attributes, to be stored in a scheduler Queue
*/
typedef struct Datum
{
    pid_t inputPID;
    time_t inputBurst;
    pid_t childPID;
} Datum;

/*======== SEE 'datum.c' FOR FUNCTION HEADER COMMENTS ========*/
Datum* initDatumStruct(Datum* datum, pid_t pid, unsigned long burst);

#endif // DATUM_H