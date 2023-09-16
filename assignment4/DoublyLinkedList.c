/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 4 (Final Project)
        Date of Submission: 9/13/2023
        Name of this file: DoublyLinkedList.c
        Short Description of contents:
            This file contains the doubly-linked-list data structure and its methods
*/
#include "DoublyLinkedList.h"

/*
    Function Name: isHeadEmpty

    Input to Method:
        DoublyLinkedList *thisDblLinkedList - the linked list whose head we want to check is initialized

    Output (Return value):
         bool - true/false as to if this linked list has a valid head node (false = it does, true = it's empty)

    Brief description of the task:
        Check if this doubly linked list has a head node initialized.
*/
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

/*
    Function Name: isTailEmpty

    Input to Method:
        DoublyLinkedList *thisDblLinkedList - the linked list whose tail we want to check is initialized

    Output (Return value):
         bool - true/false as to if this linked list has a valid tail/end node (false = it does, true = it's empty)

    Brief description of the task:
        Check if this doubly linked list has a tail node initialized.
*/
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

/*
    Function Name: isCurrTempEmpty

    Input to Method:
        DoublyLinkedList *thisDblLinkedList - the linked list whose temp node we want to check is initialized

    Output (Return value):
         bool - true/false as to if this linked list has a valid temp node (false = it does, true = it's empty)

    Brief description of the task:
        Check if this doubly linked list has a temp node initialized.
*/
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

/*
    Function Name: setHeadTo

    Input to Method:
        DoublyLinkedList *thisDblLinkedList - the linked list whose head node we want to set to a new head node

    Output (Return value):
         void - N/A

    Brief description of the task:
        Set this doubly linked list's current head node to this newly passed node
*/
void setHeadTo(DoublyLinkedList* thisDblLinkedList, DoublyLinkedNode* thisNode)
{
    thisDblLinkedList->head = thisNode;
}

/*
    Function Name: setTailTo

    Input to Method:
        DoublyLinkedList *thisDblLinkedList - the linked list whose tail node we want to set to a new tail node

    Output (Return value):
         void - N/A

    Brief description of the task:
        Set this doubly linked list's current tail/end node to this newly passed node
*/
void setTailTo(DoublyLinkedList* thisDblLinkedList, DoublyLinkedNode* thisNode)
{
    thisDblLinkedList->tail = thisNode;
}

/*
    Function Name: setTempTo

    Input to Method:
        DoublyLinkedList *thisDblLinkedList - the linked list whose temp node we want to set to a new temp node

    Output (Return value):
         void - N/A

    Brief description of the task:
        Set this doubly linked list's current temp node to this newly passed node
*/
void setTempTo(DoublyLinkedList* thisDblLinkedList, DoublyLinkedNode* thisNode)
{
    thisDblLinkedList->temp = thisNode;
}

/*
    Function Name: getTheHeadOf

    Input to Method:
        DoublyLinkedList *thisDblLinkedList - the linked list whose head node we want to fetch

    Output (Return value):
         DoublyLinkedNode* - the fetched head node (pointer) inside this doubly linked list

    Brief description of the task:
        Get this doubly linked list's current head node
*/
DoublyLinkedNode* getTheHeadOf(DoublyLinkedList* thisDblLinkedList)
{
    DoublyLinkedNode* foundHead = thisDblLinkedList->head;
    return foundHead;
}

/*
    Function Name: getTheTailOf

    Input to Method:
        DoublyLinkedList *thisDblLinkedList - the linked list whose tail node we want to fetch

    Output (Return value):
        DoublyLinkedNode* - the fetched tail node (pointer) inside this doubly linked list

    Brief description of the task:
        Get this doubly linked list's current tail node
*/
DoublyLinkedNode* getTheTailOf(DoublyLinkedList* thisDblLinkedList)
{
    DoublyLinkedNode* foundTail = thisDblLinkedList->tail;
    return foundTail;
}

/*
    Function Name: getTheTempNodeOf

    Input to Method:
        DoublyLinkedList *thisDblLinkedList - the linked list whose temp node we want to fetch

    Output (Return value):
        DoublyLinkedNode* - the fetched temp node (pointer) inside this doubly linked list

    Brief description of the task:
        Get this doubly linked list's current temp node
*/
DoublyLinkedNode* getTheTempNodeOf(DoublyLinkedList* thisDblLinkedList)
{
    DoublyLinkedNode* foundTempNode = thisDblLinkedList->temp;
    return foundTempNode;
}

/*
    Function Name: insertToTailEnd

    Input to Method:
        DoublyLinkedList *thisDblLinkedList - the doubly linked list in which we want to add a new datum node into
        int thisDatum - the value of the datum which we'll attempt to insert into this doubly linked list

    Output (Return value):
        int - an integer indicating the success or failure of this procedure; -1 = fail, 0 = all good

    Brief description of the task:
        Attempt to insert a new datum into this doubly linked list (whose node structures have already been init.),
        either as the new head of this linked list or as the new tail.
*/
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
    /*
     Else, attempt to add this new datum
     as the new tail node of this
     doubly linked list.
     */
    else
    {
        // Use the temp node to hold the old tail node which will become the new tail's previous node:
        setTempTo(thisDblLinkedList, getTheTailOf(thisDblLinkedList));
        thisDblLinkedList->tail->next = (struct DoublyLinkedNode*)malloc(sizeof(struct DoublyLinkedNode));

        // Malloc error handling:
        if (getTheNextNodeAfter(getTheTailOf(thisDblLinkedList)) == NULL)
        {
            printf("Unable to allocate enough space for tail node.\n");
            exit(EXIT_FAILURE);
            return -1;
        }

        // Re-arrange node references so that this new datum is at the tail-end of this linked list:
        setNodeDatum(thisDblLinkedList->tail->next, thisDatum);
        setTailTo(thisDblLinkedList, getTheNextNodeAfter(getTheTailOf(thisDblLinkedList)));
        setNodePrevTo(thisDblLinkedList->tail, getTheTempNodeOf(thisDblLinkedList));
    }
    // Update the drop-off after the tail of the list:
    setNodeNextTo(thisDblLinkedList->tail, NULL);
    return 0;
}

/*
    Function Name: addThisManyEmptyNodes

    Input to Method:
        DoublyLinkedList *thisDblLinkedList - the linked list to initialize a bunch of empty nodes into
        int amountOfNodes - the number of nodes to initialize inside this doubly linked list

    Output (Return value):
        void - N/A

    Brief description of the task:
        Initialize n-many amount of place-holder nodes to be used inside this linked list.
*/
void addThisManyEmptyNodes(DoublyLinkedList* thisDblLinkedList, int amountOfNodes)
{
    int status = -1;
    for (int index = 0; index < amountOfNodes; index++)
    {
        status = insertToTailEnd(thisDblLinkedList, 0);
    }
}

/*
    Function Name: freeListV2

    Input to Method:
        DoublyLinkedNode *head - the starting point to which we'll want to start the freeing process from

    Output (Return value):
        void - N/A

    Brief description of the task:
        Frees the whole linked list's nodes via starting from the head node and going through each node pointer until the end.
*/
void freeListV2(struct DoublyLinkedNode* head)
{
    struct DoublyLinkedNode* temp;

    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }

}