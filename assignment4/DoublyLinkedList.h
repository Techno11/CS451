/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 4 (Final Project)
        Date of Submission: 9/13/2023
        Name of this file: DoublyLinkedList.h
        Short Description of contents:
            This file contains the prototypes for the doubly-linked-list data structure and its methods
*/

#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "DoublyLinkedNode.c"

// Definition of a created double linked list with nodes such as head and tail inside
typedef struct DoublyLinkedList {
    struct DoublyLinkedNode* head;
    struct DoublyLinkedNode* tail;
    struct DoublyLinkedNode* temp;
} DoublyLinkedList;

/*======== SEE 'DoublyLinkedList.c' FOR FUNCTION HEADER COMMENTS ========*/

// List of functional data structure checkers:
bool isHeadEmpty(DoublyLinkedList* thisDblLinkedList);
bool isTailEmpty(DoublyLinkedList* thisDblLinkedList);
bool isCurrTempEmpty(DoublyLinkedList* thisDblLinkedList);

// List of Double Linked List mutators:
void setHeadTo(DoublyLinkedList* thisDblLinkedList, DoublyLinkedNode* thisNode);
void setTailTo(DoublyLinkedList* thisDblLinkedList, DoublyLinkedNode* thisNode);
void setTempTo(DoublyLinkedList* thisDblLinkedList, DoublyLinkedNode* thisNode);

// List of Double Linked List quick accessors:
DoublyLinkedNode* getTheHeadOf(DoublyLinkedList* thisDblLinkedList);
DoublyLinkedNode* getTheTailOf(DoublyLinkedList* thisDblLinkedList);
DoublyLinkedNode* getTheTempNodeOf(DoublyLinkedList* thisDblLinkedList);

// Have a head pointer, tail pointer, and temp pointer (for tail swap) in main
void addThisManyEmptyNodes(DoublyLinkedList* thisDblLinkedList, int amountOfNodes);
int insertToTailEnd(DoublyLinkedList* thisDblLinkedList, int thisDatum);

// Method to free linked list data structure by going through each node, starting at the head:
void freeListV2(struct DoublyLinkedNode* head);

#endif //DOUBLYLINKEDLIST_H
