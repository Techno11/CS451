//
// Created by bsting on 7/31/23.
//
#include <stdlib.h>
#include "queue.h"

void allocThisQueue(Queue* queue, size_t arraySize)
{
    queue = malloc(sizeof(*queue));
    queue->array = (pid_t*)malloc(sizeof(pid_t) * queue->arraySize);
    for (int i = 0; i < arraySize; i++)
    {
        queue->array[i] = 0;
    }
}

void freeThisQueue(Queue* queue)
{
    free(queue->array);
    free(queue);
}

bool isEmpty(Queue* thisQueue)
{
    bool emptyOrNot = (getCount(thisQueue) == 0);
    return emptyOrNot;
}

bool isFull(Queue* thisQueue)
{
    bool fullOrNot = (getCount(thisQueue) == getQueueSize(thisQueue));
    return fullOrNot;
}

size_t getQueueSize(Queue* thisQueue)
{
    size_t thisSize = thisQueue->arraySize;
    return thisSize;
}

int getRear(Queue* thisQueue)
{
    int thisRear = thisQueue->rearIndex;
    return thisRear;
}

int getFront(Queue* thisQueue)
{
    int thisFront = thisQueue->frontIndex;
    return thisFront;
}

size_t getCount(Queue* thisQueue)
{
    size_t thisCount = thisQueue->currCount;
    return thisCount;
}

pid_t getElementAt(Queue* thisQueue, int index)
{
    if (index < getQueueSize(thisQueue))
    {
        pid_t reqElement = thisQueue->array[index];
        return reqElement;
    }
    else
    {
        printf("Queue: attempted to fetch element outside of queue size! \n");
        exit(EXIT_FAILURE);
    }
}

void setQueueSize(Queue* thisQueue, size_t newSize)
{
    thisQueue->arraySize = newSize;
}

void setFront(Queue* thisQueue, int newFrontIndex)
{
    thisQueue->frontIndex = newFrontIndex;
}

void setRear(Queue* thisQueue, int newRearIndex)
{
    thisQueue->rearIndex = newRearIndex;
}

void setCount(Queue* thisQueue, size_t newCount)
{
    thisQueue->currCount = newCount;
}

void setElementAt(Queue* thisQueue, pid_t newElement, int index)
{
    if (index < getQueueSize(thisQueue)) // Can set elem. indices from 0 to size-1
    {
        thisQueue->array[index] = newElement;
    }
}

void newQueue(Queue* new_queue, size_t queueSize)
{
    allocThisQueue(new_queue, queueSize);
    setQueueSize(new_queue, queueSize);
    setFront(new_queue, -1);
    setRear(new_queue, -1);
    setCount(new_queue, 0);
}

void enqueue_Push(Queue* queue, pid_t newProc)
{
    //if (getRear(queue) == (getQueueSize(queue) - 1))
    if(isFull(queue))
    {
        printf("Queue: overflow! \n");
        exit(EXIT_FAILURE);
    }
    else
    {
        int newRear = getRear(queue);
        newRear = (newRear + 1); //;
        setRear(queue, newRear);
        setElementAt(queue, newProc, newRear);
        size_t newCount = getCount(queue);
        newCount = newCount + 1; // Update number of items in this queue
        setCount(queue, newCount);
    }
}

pid_t dequeue_Pop(Queue* queue)
{
    if (isEmpty(queue))
    {
        printf("Queue: empty! \n");
        exit(EXIT_FAILURE);
    }
    else
    {
        pid_t poppedItem = getElementAt(queue, getFront(queue));

        // OPTIONAL STEP: I'm just cleaning this PID up, to a default value of 0...
        setElementAt(queue, 0, getFront(queue)); // 0 = NULL PID

        int newFront = getFront(queue);
        newFront = (newFront + 1); //% getQueueSize(queue);
        setFront(queue, newFront);
        size_t newCount = getCount(queue);
        newCount = newCount - 1; // Update number of items in this queue
        setCount(queue, newCount);
        return poppedItem;
    }
}
