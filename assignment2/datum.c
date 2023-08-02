#include "datum.h"


Datum* initDatumStruct(Datum* datum, pid_t pid, unsigned long burst) {
{
    datum = malloc(sizeof(*datum));
    return datum;
}