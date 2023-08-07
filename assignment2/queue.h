/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 2
        Date of Submission: 7/24/2023
        Name of this file: queue.h
        Short Description of contents:
            This file contains the prototypes for the queue data structure
*/

#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <sys/types.h>
#include <stdbool.h>
#include "datum.c"

typedef struct Queue
{
    size_t arraySize;
    size_t currCount;
    size_t rearIndex;
    size_t frontIndex;
    Datum* array; // Dynamic array to accept various sizes

} Queue;

Datum* allocThisQueue(Queue* queue, size_t arraySize);
Queue* initQueueStruct(Queue* queue, size_t arraySize);
//Queue* allocThisQueue(Queue* queue, size_t arraySize);
void freeThisQueue(Queue* queue);

void newQueue(Queue* new_queue, size_t queueSize);

void enqueue_Push(Queue* queue, Datum newProc);
Datum dequeue_Pop(Queue* queue);
Datum peek(Queue* queue);
bool isEmpty(Queue* thisQueue);
bool isFull(Queue* thisQueue);

size_t getQueueSize(Queue* thisQueue);
size_t getRear(Queue* thisQueue);
size_t getFront(Queue* thisQueue);
size_t getCount(Queue* thisQueue);
Datum getElementAt(Queue* thisQueue, size_t index);

void setQueueSize(Queue* thisQueue, size_t newSize);
void setFront(Queue* thisQueue, size_t newFrontIndex);
void setRear(Queue* thisQueue, size_t newRearIndex);
void setCount(Queue* thisQueue, size_t newCount);
void setElementAt(Queue* thisQueue, Datum newElement, size_t index);

#endif //QUEUE_H