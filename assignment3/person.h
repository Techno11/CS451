/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 3
        Date of Submission: 8/99/2023
        Name of this file: person.h
        Short Description of contents:
            This file contains the prototypes for the person data structure
*/

#ifndef PERSON_H
#define PERSON_H

#include <stdio.h>
#include <sys/types.h>
#include <stdbool.h>
#include "itinerary.h"

/*
    Struct: Person

    Member variables:
        itinerary - Array of itinerary objects for the person
        currentItenerayItem - The current itinerary item the person is on

    Brief description of the structure:
        A representation of the theorhetical person
*/
typedef struct Person
{
    int currentItenerayItem;
    int itinerarySize;
    Itinerary *itinerary[];
} Person;

/*======== SEE 'person.c' FOR FUNCTION HEADER COMMENTS ========*/
Person* initPersonStruct(Person* person, int itinerarySize);

#endif // PERSON_H