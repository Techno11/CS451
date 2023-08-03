#include "datum.h"
#include <stdlib.h>


Datum* initDatumStruct(Datum* datum, pid_t pid, unsigned long burst)
{
    //datum->inputBurst = burst;
    datum = malloc(sizeof(*datum));
    datum->lastPrime = malloc(sizeof(unsigned long int));
    return datum;
}