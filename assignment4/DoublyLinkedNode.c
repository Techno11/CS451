/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 4 (Final Project)
        Date of Submission: 9/13/2023
        Name of this file: DoublyLinkedNode.c
        Short Description of contents:
            This file contains the doubly-linked-list node's data structure and its methods
*/
#include "DoublyLinkedNode.h"

/*
    Function Name: setNodeDatum

    Input to Method:
        DoublyLinkedNode *thisDblLinkedNode - the node whose datum we'll be modifying
        int newDatum - the new datum value to change inside node

    Output (Return value):
        void - N/A

    Brief description of the task:
        Set the datum member variable of this node to the newly passed datum value.
*/
void setNodeDatum(DoublyLinkedNode* thisDblLinkedNode, int newDatum)
{
    thisDblLinkedNode->datum = newDatum;
}

/*
    Function Name: setNodeNextTo

    Input to Method:
        DoublyLinkedNode *thisDblLinkedNode - this node that will be pointing to its new next node
        DoublyLinkedNode *thisNextNode - the new node to be pointed to by this node

    Output (Return value):
        void - N/A

    Brief description of the task:
        Set the next member variable of this node to the new next node to link them together.
*/
void setNodeNextTo(DoublyLinkedNode* thisDblLinkedNode, DoublyLinkedNode* thisNextNode)
{
    thisDblLinkedNode->next = thisNextNode;
}

/*
    Function Name: setNodePrevTo

    Input to Method:
        DoublyLinkedNode *thisDblLinkedNode - this node that will be pointed to by a new previous node
        DoublyLinkedNode *thisPrevNode - the new node that will be pointing to this node

    Output (Return value):
        void - N/A

    Brief description of the task:
        Set the prev member variable of this node to the new previous node to link them together.
*/
void setNodePrevTo(DoublyLinkedNode* thisDblLinkedNode, DoublyLinkedNode* thisPrevNode)
{
    thisDblLinkedNode->prev = thisPrevNode;
}

/*
    Function Name: getNodeDatum

    Input to Method:
        DoublyLinkedNode *thisDblLinkedNode - this node whose datum we want to extract

    Output (Return value):
        int - the integer value inside node's datum variable

    Brief description of the task:
        Get the datum inside this node.
*/
int getNodeDatum(DoublyLinkedNode* thisDblLinkedNode)
{
    int value = thisDblLinkedNode->datum;
    return value;
}

/*
    Function Name: getTheNextNodeAfter

    Input to Method:
        DoublyLinkedNode *thisDblLinkedNode - this node whose next node connection we want to get

    Output (Return value):
        DoublyLinkedNode* - the next node (pointer) of this node

    Brief description of the task:
        Get the next node adjacent to this node.
*/
DoublyLinkedNode* getTheNextNodeAfter(DoublyLinkedNode* thisDblLinkedNode)
{
    DoublyLinkedNode* itsNextNode = thisDblLinkedNode->next;
    return itsNextNode;
}

/*
    Function Name: getThePrevNodeBefore

    Input to Method:
        DoublyLinkedNode *thisDblLinkedNode - this node whose previous node connection we want to get

    Output (Return value):
        DoublyLinkedNode* - the previous node (pointer) of this node

    Brief description of the task:
        Get the previous node adjacently behind this node.
*/
DoublyLinkedNode* getThePrevNodeBefore(DoublyLinkedNode* thisDblLinkedNode)
{
    DoublyLinkedNode* itsPrevNode = thisDblLinkedNode->prev;
    return itsPrevNode;
}