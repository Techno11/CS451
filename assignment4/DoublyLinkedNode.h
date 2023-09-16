/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 4 (Final Project)
        Date of Submission: 9/13/2023
        Name of this file: DoublyLinkedNode.h
        Short Description of contents:
            This file contains the prototypes for the doubly-linked-list node's data structure and its methods
*/

#ifndef DOUBLYLINKEDNODE_H
#define DOUBLYLINKEDNODE_H

// Definition of a created node with double node pointers
// (one towards front, one towards tail/back)
typedef struct DoublyLinkedNode {
    int datum;
    struct DoublyLinkedNode* next;
    struct DoublyLinkedNode* prev;
} DoublyLinkedNode;

/*======== SEE 'DoublyLinkedNode.c' FOR FUNCTION HEADER COMMENTS ========*/

// List of Node mutators for Double Linked List
void setNodeDatum(DoublyLinkedNode* thisDblLinkedNode, int newDatum);
void setNodeNextTo(DoublyLinkedNode* thisDblLinkedNode, DoublyLinkedNode* thisNextNode);
void setNodePrevTo(DoublyLinkedNode* thisDblLinkedNode, DoublyLinkedNode* thisPrevNode);

// List of Node accessors for Double Linked List
int getNodeDatum(DoublyLinkedNode* thisDblLinkedNode);
DoublyLinkedNode* getTheNextNodeAfter(DoublyLinkedNode* thisDblLinkedNode);
DoublyLinkedNode* getThePrevNodeBefore(DoublyLinkedNode* thisDblLinkedNode);

#endif //DOUBLYLINKEDNODE_H
