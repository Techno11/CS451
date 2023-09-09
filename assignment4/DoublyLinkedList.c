//
// Created by bsting on 9/8/23.
//
#include "DoublyLinkedList.h"
#include <stdint.h>

bool isHeadEmpty(DoublyLinkedList* thisDblLinkedList)
{
    DoublyLinkedNode* thisHead = thisDblLinkedList->head;
    if (thisHead == NULL)
    {
        return true;
    }
    else {
        return false;
    }
}
bool isTailEmpty(DoublyLinkedList* thisDblLinkedList)
{
    DoublyLinkedNode* thisTail = thisDblLinkedList->tail;
    if (thisTail == NULL)
    {
        return true;
    }
    else {
        return false;
    }
}
bool isCurrTempEmpty(DoublyLinkedList* thisDblLinkedList)
{
    DoublyLinkedNode* thisTempNode = thisDblLinkedList->temp;
    if (thisTempNode == NULL)
    {
        return true;
    }
    else {
        return false;
    }
}

// List of Double Linked List mutators:
void setHeadTo(DoublyLinkedList* thisDblLinkedList, DoublyLinkedNode* thisNode)
{
    thisDblLinkedList->head = thisNode;
}
void setTailTo(DoublyLinkedList* thisDblLinkedList, DoublyLinkedNode* thisNode)
{
    thisDblLinkedList->tail = thisNode;
}
void setTempTo(DoublyLinkedList* thisDblLinkedList, DoublyLinkedNode* thisNode)
{
    thisDblLinkedList->temp = thisNode;
}

// List of Double Linked List quick accessors:
DoublyLinkedNode* getTheHeadOf(DoublyLinkedList* thisDblLinkedList)
{
    DoublyLinkedNode* foundHead = thisDblLinkedList->head;
    return foundHead;
}
DoublyLinkedNode* getTheTailOf(DoublyLinkedList* thisDblLinkedList)
{
    DoublyLinkedNode* foundTail = thisDblLinkedList->tail;
    return foundTail;
}
DoublyLinkedNode* getTheTempNodeOf(DoublyLinkedList* thisDblLinkedList)
{
    DoublyLinkedNode* foundTempNode = thisDblLinkedList->temp;
    return foundTempNode;
}

int insertToTailEnd(DoublyLinkedList* thisDblLinkedList, int thisDatum)
{
    /*
     If there is no head currently in the list,
     after making a new node,
     then add that new node AS THE HEAD
     */
    bool headEmpty = isHeadEmpty(thisDblLinkedList);
    if (headEmpty)
    {
        thisDblLinkedList->head = (struct DoublyLinkedNode*)malloc(sizeof(struct DoublyLinkedNode));

        // Malloc error handling:
        headEmpty = isHeadEmpty(thisDblLinkedList);
        if (headEmpty)
        {
            printf("Unable to allocate enough space for head node.\n");
            exit(EXIT_FAILURE);
            return -1;
        }

        setNodeDatum(thisDblLinkedList->head, thisDatum); //change newHead to thisDblLinkedList->head if it doesn't work
        setTailTo(thisDblLinkedList, getTheHeadOf(thisDblLinkedList));
        setNodePrevTo(thisDblLinkedList->head, NULL);
    }
    else
    {
        setTempTo(thisDblLinkedList, getTheTailOf(thisDblLinkedList));
        thisDblLinkedList->tail->next = (struct DoublyLinkedNode*)malloc(sizeof(struct DoublyLinkedNode));

        // Malloc error handling:
        if (getTheNextNodeAfter(getTheTailOf(thisDblLinkedList)) == NULL)
        {
            printf("Unable to allocate enough space for tail node.\n");
            exit(EXIT_FAILURE);
            return -1;
        }

        setNodeDatum(thisDblLinkedList->tail->next, thisDatum);
        setTailTo(thisDblLinkedList, getTheNextNodeAfter(getTheTailOf(thisDblLinkedList)));
        setNodePrevTo(thisDblLinkedList->tail, getTheTempNodeOf(thisDblLinkedList));
    }
    setNodeNextTo(thisDblLinkedList->tail, NULL);
    return 0;
}


void addThisManyEmptyNodes(DoublyLinkedList* thisDblLinkedList, int amountOfNodes)
{
    int status = -1;
    for (int index = 0; index < amountOfNodes; index++)
    {
        status = insertToTailEnd(thisDblLinkedList, 0);
    }
}

void freeThisManyDoublyLinkedNodes(DoublyLinkedList* thisDblLinkedList, int amountOfNodes)
{
    struct DoublyLinkedNode** freeingPointer = &thisDblLinkedList->head;
    for (int index = 0; index < amountOfNodes; index++)
    {
        free(*freeingPointer);
        *freeingPointer = NULL;
        freeingPointer++;
    }
}

void displayDoublyLinkedList(DoublyLinkedList* thisDblLinkedList)
{
    bool headEmpty = isHeadEmpty(thisDblLinkedList);
    if (headEmpty)
    {
        printf("Please add a node first as there's no available output to read.\n");
    }
    else {
        setTempTo(thisDblLinkedList, getTheHeadOf(thisDblLinkedList));
        while (!isCurrTempEmpty(thisDblLinkedList)) {
            printf("[%d]->", getNodeDatum(getTheTempNodeOf(thisDblLinkedList)));
            setTempTo(thisDblLinkedList, getTheNextNodeAfter(getTheTempNodeOf(thisDblLinkedList)));
        }
        printf("NULL\n");
    }
}