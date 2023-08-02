#include "datum.h"
#include <stdlib.h>


Datum* initDatumStruct(Datum* datum, pid_t pid, unsigned long burst)
{
    //datum->inputPID = pid;
    //datum->inputBurst = burst;
    datum = malloc(sizeof(*datum));
    return datum;
}