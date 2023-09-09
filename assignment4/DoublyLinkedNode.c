//
// Created by bsting on 9/8/23.
//
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