//
// Created by bsting on 9/8/23.
//

#ifndef CS451_GITCLONEV2_DOUBLYLINKEDLIST_H
#define CS451_GITCLONEV2_DOUBLYLINKEDLIST_H

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
void freeThisManyDoublyLinkedNodes(DoublyLinkedList* thisDblLinkedList, int amountOfNodes);
int insertToTailEnd(DoublyLinkedList* thisDblLinkedList, int thisDatum);
void displayDoublyLinkedList(DoublyLinkedList* thisDblLinkedList);


#endif //CS451_GITCLONEV2_DOUBLYLINKEDLIST_H
