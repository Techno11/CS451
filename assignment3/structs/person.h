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
        currentItineraryItem - The current itinerary item the person is on (represented as an int index)

    Brief description of the structure:
        A representation of the theoretical person
*/
typedef struct Person
{
    int currentItineraryItem; // if currentItineraryItem == itinerarySize then itinerary is empty/done
    int keyID;
    int itinerarySize;
    Itinerary *itinerary[];
} Person;

/*======== SEE 'person.c' FOR FUNCTION HEADER COMMENTS ========*/
Person *initPersonStruct(Person *person, int itinerarySize);
void setPersonKey(Person *person, int newKeyID);
int getPersonKey(Person *person);
int getItinerarySize(Person *person);
int getCurrentItineraryItemInd(Person *person);
Itinerary *getCurrentItineraryItem(Person *person);
Itinerary *getNextItineraryItem(Person *person);
bool isItineraryDone(Person *person);

#endif // PERSON_H