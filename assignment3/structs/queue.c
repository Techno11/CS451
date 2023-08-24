/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 2
        Date of Submission: 8/9/2023
        Name of this file: queue.c
        Short Description of contents:
            This file contains the functions for the queue data structure
*/

#include <stdlib.h>
#include "queue.h"

/*
    Function Name: allocThisQueue

    Input to Method:
        queue - The queue structure that has the array to dynamically allocate memory to
        arraySize - The length of the queue's array

    Output (Return value):
        The new reference to the allocated queue array, of type Datum

    Brief description of the task:
        Allocates memory to the queue's array
*/
Datum* allocThisQueue(Queue* queue, size_t arraySize)
{
    queue->array = (Datum*)malloc(sizeof(Datum) * arraySize);

    return queue->array;
}

/*
    Function Name: initQueueStruct

    Input to Method:
        queue - The queue structure to dynamically allocate memory to
        arraySize - The length of the queue's array

    Output (Return value):
        The new reference to the allocated queue

    Brief description of the task:
        Allocates memory to the queue's structure
*/
Queue* initQueueStruct(Queue* queue, size_t arraySize)
{
    queue = malloc(sizeof(*queue) + (arraySize * sizeof *queue->array));
    return queue;
}

/*
    Function Name: freeThisQueue

    Input to Method:
        queue - The queue structure with the encapsulated array to free dynamic memory from

    Output (Return value):
        None/void

    Brief description of the task:
        Frees the memory allocated to this queue's array
*/
void freeThisQueue(Queue* queue)
{
    free(queue->array);
}

/*
    Function Name: isEmpty

    Input to Method:
        thisQueue - The queue structure to be checked if it's empty or not

    Output (Return value):
        True/false as to if this queue is empty or not

    Brief description of the task:
        Checks if the passed queue is empty or not
*/
bool isEmpty(Queue* thisQueue)
{
    bool emptyOrNot = (getCount(thisQueue) == 0);
    return emptyOrNot;
}

/*
    Function Name: isFull

    Input to Method:
        thisQueue - The queue structure to be checked if it's full or not

    Output (Return value):
        True/false as to if this queue is full or not

    Brief description of the task:
        Checks if the passed queue is full (at max item capacity) or not
*/
bool isFull(Queue* thisQueue)
{
    bool fullOrNot = (getCount(thisQueue) == getQueueSize(thisQueue));
    return fullOrNot;
}

/*
    Function Name: getQueueSize

    Input to Method:
        thisQueue - The queue structure whose array size is to be fetched

    Output (Return value):
        An unsigned long size_t number indicating the array size inside the queue

    Brief description of the task:
        Gets the size of this queue's array as a size_t number
*/
size_t getQueueSize(Queue* thisQueue)
{
    size_t thisSize = thisQueue->arraySize;
    return thisSize;
}

/*
    Function Name: getRear

    Input to Method:
        thisQueue - The queue structure whose rear/back index will be fetched

    Output (Return value):
        An unsigned long size_t number indicating the index position of the queue's rear

    Brief description of the task:
        Gets the rear index of this passed queue
*/
size_t getRear(Queue* thisQueue)
{
    size_t thisRear = thisQueue->rearIndex;
    return thisRear;
}

/*
    Function Name: getFront

    Input to Method:
        thisQueue - The queue structure whose front index will be fetched

    Output (Return value):
        An unsigned long size_t number indicating the index position of the queue's front

    Brief description of the task:
        Gets the front index of this passed queue
*/
size_t getFront(Queue* thisQueue)
{
    size_t thisFront = thisQueue->frontIndex;
    return thisFront;
}

/*
    Function Name: getCount

    Input to Method:
        thisQueue - The queue structure whose count of items will be fetched

    Output (Return value):
        An unsigned long size_t number indicating the number of items in the queue's array

    Brief description of the task:
        Gets the count of array items in this passed queue
*/
size_t getCount(Queue* thisQueue)
{
    size_t thisCount = thisQueue->currCount;
    return thisCount;
}

/*
    Function Name: getElementAt

    Input to Method:
        thisQueue - The queue structure to fetch an indexed element from
        index - The index indicating the position of the Datum item to get inside queue's array

    Output (Return value):
        The Datum item that was fetched at an index within this passed queue

    Brief description of the task:
        Gets a Datum item element at a valid index inside this queue's array
*/
Datum getElementAt(Queue* thisQueue, size_t index)
{
    if (index < getQueueSize(thisQueue))
    {
        Datum reqElement = thisQueue->array[index];
        return reqElement;
    }
    else
    {
        printf("Queue: attempted to fetch element outside of queue size! \n");
        exit(EXIT_FAILURE);
    }
}

/*
    Function Name: setQueueSize

    Input to Method:
        thisQueue - The queue structure whose array size will be assigned/re-assigned
        newSize - The new size of the queue array

    Output (Return value):
        None/void

    Brief description of the task:
        Sets the queue's array size to a new specified size
*/
void setQueueSize(Queue* thisQueue, size_t newSize)
{
    thisQueue->arraySize = newSize;
}

/*
    Function Name: setFront

    Input to Method:
        thisQueue - The queue structure whose front index will be assigned/re-assigned
        newFrontIndex - The new front index of the queue's array

    Output (Return value):
        None/void

    Brief description of the task:
        Sets the queue's array front index to a new specified front index
*/
void setFront(Queue* thisQueue, size_t newFrontIndex)
{
    thisQueue->frontIndex = newFrontIndex;
}

/*
    Function Name: setRear

    Input to Method:
        thisQueue - The queue structure whose rear index will be assigned/re-assigned
        newRearIndex - The new rear index of the queue's array

    Output (Return value):
        None/void

    Brief description of the task:
        Sets the queue's array rear index to a new specified rear index
*/
void setRear(Queue* thisQueue, size_t newRearIndex)
{
    thisQueue->rearIndex = newRearIndex;
}

/*
    Function Name: setCount

    Input to Method:
        thisQueue - The queue structure whose array item count will be assigned/re-assigned
        newCount - The new item count of the queue's array

    Output (Return value):
        None/void

    Brief description of the task:
        Sets the queue's array item count to a new specified count
*/
void setCount(Queue* thisQueue, size_t newCount)
{
    thisQueue->currCount = newCount;
}

/*
    Function Name: setElementAt

    Input to Method:
        thisQueue - The queue structure whose indexed array element will be assigned/re-assigned
        newElement - The new Datum item to replace at the specified index position in this queue's array
        index - The index position that specifies the Datum element to be replaced with the new element

    Output (Return value):
        None/void

    Brief description of the task:
        Sets the specified Datum element in the queue's array to the (new) passed Datum element
*/
void setElementAt(Queue* thisQueue, Datum newElement, size_t index)
{
    if (index < getQueueSize(thisQueue)) // Can set elem. indices from 0 to size-1
    {
        thisQueue->array[index] = newElement;
    }
}

/*
    Function Name: newQueue

    Input to Method:
        new_queue - The queue structure to fully construct with its own array
        queueSize - The size of the queue's newly constructed array

    Output (Return value):
        None/void

    Brief description of the task:
        Constructs a new queue that has its array allocated and member variables setup
*/
void newQueue(Queue* new_queue, size_t queueSize)
{
    /*
        To make/construct a new initialized queue:
        -1. we allocate the proper memory to the array pointer inside the queue struct,
        -2. we set the queue's (array) size to whatever was passed to this constructor,
        -3. set the queue's stored front index (in its struct) to 0, representing the very front of queue's array,
        -4. set the queue's stored rear index (in its struct) to [array size - 1] or last possible element position,
        -5. and then just set the element count member variable to 0 since the queue just got constructed!
    */
    new_queue->array = allocThisQueue(new_queue, queueSize);
    setQueueSize(new_queue, queueSize);
    setFront(new_queue, 0);
    setRear(new_queue, getQueueSize(new_queue) - 1);
    setCount(new_queue, 0);
}

/*
    Function Name: enqueue_Push

    Input to Method:
        queue - The queue structure to push a new (process) element onto
        newProc - A Datum representation of a process that will be pushed onto the passed queue

    Output (Return value):
        None/void

    Brief description of the task:
        Pushes a new process Datum element in the rear of this queue (following FCFS)
*/
void enqueue_Push(Queue* queue, Datum newProc)
{
    // Make sure we have room to push before actually pushing:
    if(isFull(queue))
    {
        printf("Queue: overflow! \n");
        exit(EXIT_FAILURE);
    }
    else
    {
        /*
         To actually push onto the queue array:
            -1. we get the rear index,
            -2. calculate the new rear index by incrementing with remainder(%) to keep array circular
            -3. set the queue's stored rear index (in its struct) to the new rear index calculated,
            -4. and then update the count member variable inside queue to reflect that an item has been pushed!
         */
        size_t newRear = getRear(queue);
        newRear = (newRear + 1) % getQueueSize(queue);
        setRear(queue, newRear);
        setElementAt(queue, newProc, newRear);
        size_t newCount = getCount(queue);
        newCount = newCount + 1; // Update number of items in this queue
        setCount(queue, newCount);
    }
}

/*
    Function Name: dequeue_Pop

    Input to Method:
        queue - The queue structure to pop the existing front (process) element from

    Output (Return value):
        The Datum element that was the front item popped off the queue

    Brief description of the task:
        Pops a process Datum element at the front of this queue (following FCFS)
*/
Datum dequeue_Pop(Queue* queue)
{
    // Make sure there's actually something to pop before popping:
    if (isEmpty(queue))
    {
        printf("Queue: empty! \n");
        exit(EXIT_FAILURE);
    }
    else
    {
        // We pop from the front of the queue in FCFS/FIFO, so the popped element is obtained at the front:
        Datum poppedItem = getElementAt(queue, getFront(queue));

        /*
         To actually pop off the queue array now:
            -1. we get the front index,
            -2. calculate the new front index by incrementing with remainder(%) to keep array circular
            -3. set the queue's stored front index (in its struct) to the new front index calculated,
            -4. update the count member variable inside queue to reflect that an item has been popped,
            -5. and then just return the popped item from the queue's array!
         */
        size_t newFront = getFront(queue);
        newFront = (newFront + 1) % getQueueSize(queue);
        setFront(queue, newFront);
        size_t newCount = getCount(queue);
        newCount = newCount - 1; // Update number of items in this queue
        setCount(queue, newCount);
        return poppedItem;
    }
}

/*
    Function Name: peek

    Input to Method:
        queue - The queue structure to which to peek into the current front (process) element

    Output (Return value):
        The Datum element that was peeked at in the queue

    Brief description of the task:
        Peeks into the passed queue to find out what the front item in the queue's array is (without popping)
*/
Datum peek(Queue* queue)
{
    if (isEmpty(queue))
    {
        printf("Queue: empty! \n");
        exit(EXIT_FAILURE);
    }
    else
    {
        // Peek and get the front item of the queue:
        return getElementAt(queue, getFront(queue));
    }
}