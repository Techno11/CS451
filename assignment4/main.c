/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 4 (Final Project)
        Date of Submission: 9/13/2023
        Name of this file: main.c
        Short Description of contents:
            Virtual memory manager
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "DoublyLinkedList.c"

// Constants as defined in assignment description
#define FRAME_SIZE 16
#define NUM_FRAMES 8
#define PAGE_SIZE (FRAME_SIZE)
#define NUM_PAGES (512 / 16) // 32

// Max number of addresses to read from addresses.txt
#define MAX_ADDRESSES 300

/*
    Function Name: readIntoRam

    Input to Method:
        int pageNumber - the page number to read
        int frameNumber - the frame number to read into

    Output (Return value):
        N/A (void)

    Brief description of the task:
        Read the given page number into RAM
*/
void readIntoRam(char ram[NUM_FRAMES * FRAME_SIZE], int pageNumber, int frameNumber)
{
    // Calculate start of page (x2 because of \n)
    int startOfPage = pageNumber * PAGE_SIZE * 2;

    // Load page file from disk
    FILE *processFile = fopen("PROCESS.txt", "r");

    // Seek to the correct page
    fseek(processFile, startOfPage, SEEK_SET);

    // Location in RAM to start writing to
    int ramStart = frameNumber * FRAME_SIZE;

    // Read page into ram, skipping \n
    for (int i = 0; i < PAGE_SIZE; i++)
    {
        // Read char from file into RAM
        ram[ramStart + i] = fgetc(processFile);

        // If \n, decrement i so that we overwrite it
        if (ram[ramStart + i] == '\n')
        {
            i--;
        }
    }

    // Close the file
    fclose(processFile);
}

/*
    Function Name: searchLRUCache

    Input to Method:
        DoublyLinkedList *LRUStack - the LRU stack to search through
        int targetDatum - the data to search for

    Output (Return value):
        int - 0 if successful, -1 if not

    Brief description of the task:
        Search through a doubly linked list LRU-style, finding the target datum and moving it to the head.
        If the target datum is not found, move all elements to the right and place the target datum at the head.
*/
int searchLRUCache(DoublyLinkedList *LRUStack, int targetDatum)
{
    bool headEmpty = isHeadEmpty(LRUStack);
    if (LRUStack->head == NULL)
    {
        printf("No available nodes to search through; please add one first.\n");
        return -1;
    }

    // Store head temporarily:
    setTempTo(LRUStack, getTheHeadOf(LRUStack));

    // Traverse!
    while (!isCurrTempEmpty(LRUStack))
    {
        if (getNodeDatum(LRUStack->temp) == targetDatum)
        {
            // Shift all values before the target spot to the right (towards tail):
            while (LRUStack->temp != LRUStack->head)
            {
                setNodeDatum(LRUStack->temp, LRUStack->temp->prev->datum);
                setTempTo(LRUStack, getThePrevNodeBefore(LRUStack->temp));
            }

            // Place target datum at the head (it was freshly used):
            setNodeDatum(LRUStack->head, targetDatum);
            return 0;
        }
        // Keep going until we find the target:
        setTempTo(LRUStack, getTheNextNodeAfter(LRUStack->temp));
    }

    // For if we are adding elements at the start:
    setTempTo(LRUStack, LRUStack->tail->prev);

    // Shift all values to the right towards tail and overwrite the last guy (head):
    while (!isCurrTempEmpty(LRUStack))
    {
        setNodeDatum(LRUStack->temp->next, getNodeDatum(LRUStack->temp));
        setTempTo(LRUStack, LRUStack->temp->prev);
    }
    setNodeDatum(LRUStack->head, targetDatum);
    return 0;
}

/*
    Function Name: main

    Input to Method:
        int argc - the count/number of arguments entered from the command line
        char* argv[] - the array holding character-array strings where each argument from CMD is an array element

    Output (Return value):
        An integer representing successful completion of the main method.

    Brief description of the task:
        The main method run the core program, which is to simulate a virtual memory manager.

*/
int main()
{
    // RAM
    char ram[NUM_FRAMES * FRAME_SIZE];

    // Page table, tracks page frame number.  Any pages that are loaded into RAM are > -1, unloaded pages are -1
    int pageTable[NUM_PAGES]; // Array with 32 entries

    // Track which pages are loaded in which frames.  A frame that is empty is -1
    int frameTable[NUM_FRAMES]; // Array with 8 entries

    // Track page faults
    int pageFaultCount = 0;
    int faults[MAX_ADDRESSES];

    // Initialize page table and add empty nodes
    struct DoublyLinkedList *lruFrame = malloc(sizeof(DoublyLinkedList) * (NUM_FRAMES * sizeof(DoublyLinkedNode)));
    addThisManyEmptyNodes(lruFrame, NUM_FRAMES);

    // Initialize page table with all entries as invalid
    for (int i = 0; i < NUM_PAGES; i++)
    {
        pageTable[i] = -1;
    }

    // Initialize frame table with all entries as empty
    for (int i = 0; i < NUM_FRAMES; i++)
    {
        frameTable[i] = -1;

        // Add each available frame to the LRU
        searchLRUCache(lruFrame, i);
    }

    // Open addresses.txt
    FILE *addressesFile = fopen("addresses.txt", "r");
    if (!addressesFile)
    {
        printf("Error opening addresses.txt\n");
        return 1;
    }

    // Print header
    printf("Logical Address   Physical Address  Data\n");

    // Store each line read from addresses.txt
    int logicalAddress;

    // Store total number of addresses
    float totalAddresses = 0;

    // Read addresses.txt line by line
    while (fscanf(addressesFile, "%d\n", &logicalAddress) != EOF)
    {
        // Calculate page number and offset
        int pageNumber = logicalAddress / PAGE_SIZE;
        int offset = logicalAddress % PAGE_SIZE;

        // Get frame number from page table (if it is -1, page is unloaded)
        int frameNumber = pageTable[pageNumber];

        // Store physical address
        int physicalAddress;

        // Check if the page is loaded in a frame (frame number != -1)
        if (frameNumber != -1)
        {
            // Frame is loaded. Update least-recently-used (LRU) to reflect that this frame was just used
            searchLRUCache(lruFrame, frameNumber);

            // Calculate physical address
            physicalAddress = frameNumber * FRAME_SIZE + offset;
        }
        else
        {
            // Add to page fault list
            faults[pageFaultCount] = pageNumber;

            // Update page fault count
            pageFaultCount++;

            // Get least-recently-used frame # to load page into
            frameNumber = getNodeDatum(getTheTailOf(lruFrame));

            // Get page number that is at that current frame
            int pageNumToInvalidate = frameTable[frameNumber];

            // Invalidate least-recently-used page
            pageTable[pageNumToInvalidate] = -1;

            // Update least-recently-used (LRU) to reflect that this frame was just used
            searchLRUCache(lruFrame, frameNumber);

            // Calculate physical address
            physicalAddress = frameNumber * FRAME_SIZE + offset;

            // Load page into RAM frame
            readIntoRam(ram, pageNumber, frameNumber);

            // Update frame number in page table, as page is now loaded in frame
            pageTable[pageNumber] = frameNumber;

            // Update page number in frame table, as frame now contains page
            frameTable[frameNumber] = pageNumber;
        }

        // Data is confirmed to be in RAM, lets read it
        char data = ram[physicalAddress];

        // Print logical address, physical address, and data
        printf("%-17d %-17d %c\n", logicalAddress, physicalAddress, data);

        // Increment total addresses
        totalAddresses++;
    }

    // Close addresses.txt
    fclose(addressesFile);

    // Convert page fault count and num pages to float
    float pfc = pageFaultCount;

    // Calculate page fault rate
    float pageFaultRate = (pfc / totalAddresses) * 100;

    // Print page fault list
    printf("\nPage fault list (in order):\n");
    for (int i = 0; i < pageFaultCount; i++)
    {
        printf("%d", faults[i]);
        if (i != pageFaultCount - 1)
        {
            printf(", ");
        }
    }
    printf("\n");

    // Print number of addresses
    printf("\nNumber of addresses: %.0f\n", totalAddresses);

    // Print Number of page faults
    printf("Number of page faults: %d\n", pageFaultCount);

    // Print page fault rate
    printf("\nPage-fault rate: %.2f%%\n", pageFaultRate);

    // Free linked list
    freeListV2(lruFrame->head);
    free(lruFrame);

    // Exit
    return 0;
}