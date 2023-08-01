//
// Created by bsting on 7/31/23.
//

#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <sys/types.h>
#include <stdbool.h>

typedef struct Queue
{
    size_t arraySize;
    size_t currCount;
    size_t rearIndex;
    size_t frontIndex;
    pid_t* array; // Dynamic array to accept various sizes

} Queue;

pid_t* allocThisQueue(Queue* queue, size_t arraySize);
Queue* initStruct(Queue* queue, size_t arraySize);
//Queue* allocThisQueue(Queue* queue, size_t arraySize);
void freeThisQueue(Queue* queue);

void newQueue(Queue* new_queue, size_t queueSize);

void enqueue_Push(Queue* queue, pid_t newProc);
pid_t dequeue_Pop(Queue* queue);
pid_t peek(Queue* queue);
bool isEmpty(Queue* thisQueue);
bool isFull(Queue* thisQueue);

size_t getQueueSize(Queue* thisQueue);
size_t getRear(Queue* thisQueue);
size_t getFront(Queue* thisQueue);
size_t getCount(Queue* thisQueue);
pid_t getElementAt(Queue* thisQueue, size_t index);

void setQueueSize(Queue* thisQueue, size_t newSize);
void setFront(Queue* thisQueue, size_t newFrontIndex);
void setRear(Queue* thisQueue, size_t newRearIndex);
void setCount(Queue* thisQueue, size_t newCount);
void setElementAt(Queue* thisQueue, pid_t newElement, size_t index);

#endif //QUEUE_H
