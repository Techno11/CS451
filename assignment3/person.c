/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 3
        Date of Submission: 8/99/2023
        Name of this file: person.c
        Short Description of contents:
            This file contains the person data structure
*/
#include "person.h"
#include <stdlib.h>

/*
    Struct: Person

    Member variables:
        person - person to be initialized

    Brief description of the structure:
        A representation of a person and their itenerary
*/
Person* initPersonStruct(Person* person, int itinerarySize)
{
    person->currentItenerayItem = 0;
    person->itinerarySize = itinerarySize;
    for(int i = 0; i < itinerarySize; i++)
    {
        person->itinerary[i] = NULL;
    }

    return person;
}