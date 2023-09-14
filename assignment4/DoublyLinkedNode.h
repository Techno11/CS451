/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 4 (Final Project)
        Date of Submission: 9/13/2023
        Name of this file: DoublyLinkedNode.h
        Short Description of contents:
            This file contains the prototypes for the doublely-linked-list node's data structure and its methods 
*/

#ifndef CS451_GITCLONEV2_DOUBLYLINKEDNODE_H
#define CS451_GITCLONEV2_DOUBLYLINKEDNODE_H

// Definition of a created node with double node pointers
// (one towards front, one towards tail/back)
typedef struct DoublyLinkedNode {
    int datum;
    struct DoublyLinkedNode* next;
    struct DoublyLinkedNode* prev;
} DoublyLinkedNode;

void setNodeDatum(DoublyLinkedNode* thisDblLinkedNode, int newDatum);
void setNodeNextTo(DoublyLinkedNode* thisDblLinkedNode, DoublyLinkedNode* thisNextNode);
void setNodePrevTo(DoublyLinkedNode* thisDblLinkedNode, DoublyLinkedNode* thisPrevNode);

int getNodeDatum(DoublyLinkedNode* thisDblLinkedNode);
DoublyLinkedNode* getTheNextNodeAfter(DoublyLinkedNode* thisDblLinkedNode);
DoublyLinkedNode* getThePrevNodeBefore(DoublyLinkedNode* thisDblLinkedNode);

#endif //CS451_GITCLONEV2_DOUBLYLINKEDNODE_H
