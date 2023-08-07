/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 2
        Date of Submission: 7/24/2023
        Name of this file: datum.c
        Short Description of contents:
            This file contains the structure for the datum data structure
*/

#include "datum.h"
#include <stdlib.h>

/*
    Function Name: initDatumStruct

    Input to Method:
        int sigNum - Signal number which triggered this handler

    Output (Return value):
        N/A

    Brief description of the task:
        Initilize the datum struct
*/
Datum* initDatumStruct(Datum* datum, pid_t pid, unsigned long burst)
{
    //datum->inputBurst = burst;
    datum = malloc(sizeof(*datum));
    return datum;
}