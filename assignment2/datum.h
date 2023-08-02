#ifndef DATUM_H
#define DATUM_H

#include <stdio.h>
#include <sys/types.h>
#include <stdbool.h>

typedef struct Datum
{
    pid_t inputPID;
    unsigned long inputBurst;
    pid_t* childPID;
} Datum;


Datum* initDatumStruct(Datum* datum, pid_t pid, unsigned long burst);

#endif