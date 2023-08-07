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
    unsigned long int* lastPrime;
} Datum;


Datum* initDatumStruct(Datum* datum, pid_t pid, unsigned long burst);

#endif