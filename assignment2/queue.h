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
    int rearIndex;
    int frontIndex;
    pid_t* array; // Dynamic array to accept various sizes

} Queue;

void allocThisQueue(Queue* queue, size_t arraySize);
void freeThisQueue(Queue* queue);

void newQueue(Queue* new_queue, size_t queueSize);

void enqueue_Push(Queue* queue, pid_t newProc);
pid_t dequeue_Pop(Queue* queue);
bool isEmpty(Queue* thisQueue);
bool isFull(Queue* thisQueue);

size_t getQueueSize(Queue* thisQueue);
int getRear(Queue* thisQueue);
int getFront(Queue* thisQueue);
size_t getCount(Queue* thisQueue);
pid_t getElementAt(Queue* thisQueue, int index);

void setQueueSize(Queue* thisQueue, size_t newSize);
void setFront(Queue* thisQueue, int newFrontIndex);
void setRear(Queue* thisQueue, int newRearIndex);
void setCount(Queue* thisQueue, size_t newCount);
void setElementAt(Queue* thisQueue, pid_t newElement, int index);

#endif //QUEUE_H
