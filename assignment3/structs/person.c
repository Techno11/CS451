/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 3
        Date of Submission: 8/99/2023
        Name of this file: person.c
        Short Description of contents:
            This file contains the person data structure and its methods
*/
#include "person.h"
#include <stdlib.h>

/*
    Struct: Person

    Member variables:
        person - person to be initialized

    Brief description of the structure:
        A representation of a person and their itinerary
*/
Person *initPersonStruct(Person *person, int itinerarySize)
{
    person->currentItineraryItem = 0;
    person->keyID = -1;
    person->itinerarySize = itinerarySize;
    for (int i = 0; i < itinerarySize; i++)
    {
        person->itinerary[i] = NULL;
    }

    return person;
}

/*
    Function Name: setPersonKey

    Input to Method:
        Person *person - The person to set the key for
        int newKeyID - The new key to set for the person

    Output (Return value):
        N/A (void)

    Brief description of the task:
        Update the person key in a given person struct
*/
void setPersonKey(Person *person, int newKeyID)
{
    person->keyID = newKeyID;
}

/*
    Function Name: getPersonKey

    Input to Method:
        Person *person - The person to get the key for

    Output (Return value):
        int - The key of the person

    Brief description of the task:
        Read the person key in a given person struct
*/
int getPersonKey(Person *person)
{
    int key = person->keyID;
    return key;
}

/*
    Function Name: getItinerarySize

    Input to Method:
        Person *person - The person to get the itinerary size for

    Output (Return value):
        int - The itinerary size of the person

    Brief description of the task:
        Read the itinerary size in a given person struct
*/
int getItinerarySize(Person *person)
{
    int fetchedSize = person->itinerarySize;
    return fetchedSize;
}

/*
    Function Name: getCurrentItineraryItemInd

    Input to Method:
        Person *person - The person to get the itinerary index for

    Output (Return value):
        int - The current itinerary index of the person

    Brief description of the task:
        Read the current itinerary index in a given person struct
*/
int getCurrentItineraryItemInd(Person *person)
{
    int currItemIndex = person->currentItineraryItem;
    return currItemIndex;
}

/*
    Function Name: getCurrentItineraryItem

    Input to Method:
        Person *person - The person to get the itinerary item for

    Output (Return value):
        Itinerary* - The current itinerary item of the person

    Brief description of the task:
        Read the current itinerary item in a given person struct
*/
Itinerary *getCurrentItineraryItem(Person *person)
{
    int currItemIndex = getCurrentItineraryItemInd(person);
    Itinerary *currItineraryItem = person->itinerary[currItemIndex];
    return currItineraryItem;
}

/*
    Function Name: getNextItineraryItem

    Input to Method:
        Person *person - The person to get the next itinerary item for

    Output (Return value):
        Itinerary* - The next itinerary item of the person

    Brief description of the task:
        Read the next itinerary item in a given person struct
*/
Itinerary *getNextItineraryItem(Person *person)
{
    int nextItemIndex = getCurrentItineraryItemInd(person) + 1;
    if (nextItemIndex >= getItinerarySize(person))
    {
        return NULL;
    }
    else
    {
        Itinerary *currItineraryItem = person->itinerary[nextItemIndex];
        return currItineraryItem;
    }
}

/*
    Function Name: isItineraryDone

    Input to Method:
        Person *forThisPerson - The person to check if the itinerary is done for

    Output (Return value):
        bool - Whether or not the itinerary is done

    Brief description of the task:
        Check if the itinerary is done for a given person struct
*/
bool isItineraryDone(Person *forThisPerson)
{
    // If currentItineraryItem is at the itinerarySize then Person's itinerary is empty/done:
    int currentSize = getItinerarySize(forThisPerson);
    int currentItineraryItemIndex = getCurrentItineraryItemInd(forThisPerson);
    if (currentItineraryItemIndex == currentSize)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*
    Function Name: isItineraryDone

    Input to Method:
        Person *person - The person to advance the itinerary of

    Output (Return value):
        bool - Whether or not the itinerary was successfully advanced

    Brief description of the task:
        Advance the itinerary of a given person struct
*/
bool advanceItinerary(Person *person)
{
    int currentItineraryItemIndex = person->currentItineraryItem;
    int itinerarySize = person->itinerarySize;
    if (currentItineraryItemIndex == itinerarySize)
    {
        return false;
    }
    else
    {
        person->currentItineraryItem++;
        return true;
    }
}