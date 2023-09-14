/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 4 (Final Project)
        Date of Submission: 9/13/2023
        Name of this file: DoublyLinkedNode.c
        Short Description of contents:
            This file contains the doublely-linked-list node's data structure and its methods 
*/
#include "DoublyLinkedNode.h"

void setNodeDatum(DoublyLinkedNode* thisDblLinkedNode, int newDatum)
{
    thisDblLinkedNode->datum = newDatum;
}

void setNodeNextTo(DoublyLinkedNode* thisDblLinkedNode, DoublyLinkedNode* thisNextNode)
{
    thisDblLinkedNode->next = thisNextNode;
}

void setNodePrevTo(DoublyLinkedNode* thisDblLinkedNode, DoublyLinkedNode* thisPrevNode)
{
    thisDblLinkedNode->prev = thisPrevNode;
}

int getNodeDatum(DoublyLinkedNode* thisDblLinkedNode)
{
    int value = thisDblLinkedNode->datum;
    return value;
}

DoublyLinkedNode* getTheNextNodeAfter(DoublyLinkedNode* thisDblLinkedNode)
{
    DoublyLinkedNode* itsNextNode = thisDblLinkedNode->next;
    return itsNextNode;
}

DoublyLinkedNode* getThePrevNodeBefore(DoublyLinkedNode* thisDblLinkedNode)
{
    DoublyLinkedNode* itsPrevNode = thisDblLinkedNode->prev;
    return itsPrevNode;
}