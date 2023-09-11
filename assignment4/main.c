#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "DoublyLinkedList.c"

#define FRAME_SIZE 16
#define NUM_FRAMES 8
#define PAGE_SIZE (FRAME_SIZE)
#define NUM_PAGES 512 / 16 // 32

#define PROCESS_SIZE 512                             // 2^9 or 512 bytes
#define PROCESS_PAGE_SIZE (PROCESS_SIZE / PAGE_SIZE) // 32

typedef struct
{
    bool valid;
} PageTableEntry;

typedef struct
{
    int pageNumber;
} Frame;

int pageFaultCount = 0;
int currentFrame = 0;

// Track which pages are valid
PageTableEntry pageTable[NUM_PAGES]; // Array with 32 entries

// Track which pages are loaded in which frames
Frame frameTable[NUM_FRAMES];        // Array with 8 entries

// Search through a doubly linked list LRU-style:
int searchLRUCache(DoublyLinkedList *LRUStack, int targetDatum)
{
    bool headEmpty = isHeadEmpty(LRUStack);
    if (headEmpty)
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

void LRUOperation(int array[], DoublyLinkedList *LRUList, int amountOfElements)
{
    for (int index = 0; index < amountOfElements; index++)
    {
        searchLRUCache(LRUList, array[index]);
        displayDoublyLinkedList(LRUList);
    }
}

int getFrameNumFromPageNum(int pageNum)
{
    for (int i = 0; i < NUM_FRAMES; i++)
    {
        if (frameTable[i].pageNumber == pageNum)
        {
            return i;
        }
    }
    return -1;
}

int main()
{
    // TEST THE DOUBLY LINKED LIST:
    // printf("---TESTING LRU ALGORITHM:---\n");
    // int testSize = 5;
    // struct DoublyLinkedList* newLRU = malloc(sizeof(DoublyLinkedList) * (testSize * sizeof(DoublyLinkedNode)));
    // addThisManyEmptyNodes(newLRU, testSize);

    // int testArr[] = {1,2,3,4,5,2,10,7,11,1};

    // LRUOperation(testArr, newLRU, 10);

    // freeThisManyDoublyLinkedNodes(newLRU, testSize);
    // free(newLRU);

    // printf("\n---END OF LRU TESTING---\n\n");

    // Initialize page table
    struct DoublyLinkedList *lruFrame = malloc(sizeof(DoublyLinkedList) * (NUM_FRAMES * sizeof(DoublyLinkedNode)));
    addThisManyEmptyNodes(lruFrame, NUM_FRAMES);

    // Initialize RAM
    char ram[NUM_FRAMES * FRAME_SIZE];

    // Set all RAM to null
    for (int i = 0; i < NUM_FRAMES * FRAME_SIZE; i++)
    {
        ram[i] = '\0';
    }

    int logicalAddress;

    // Initialize page table with all entries as invalid
    for (int i = 0; i < NUM_PAGES; i++)
    {
        pageTable[i].valid = false;
    }

    // Initilize frame table with all entries as empty
    for (int i = 0; i < NUM_FRAMES; i++)
    {
        frameTable[i].pageNumber = -1;
    }

    // Open addresses.txt
    FILE *addressesFile = fopen("addresses.txt", "r");
    if (!addressesFile)
    {
        printf("Error opening addresses.txt\n");
        return 1;
    }

    // Read addresses.txt line by line
    while (fscanf(addressesFile, "%d\n", &logicalAddress) != EOF)
    {
        // Calculate page number and offset
        int pageNumber = logicalAddress / PAGE_SIZE;
        int offset = logicalAddress % PAGE_SIZE;

        // Check if the page is valid in the page table
        if (pageTable[pageNumber].valid == false)
        {
            // Update page table
            pageTable[pageNumber].valid = true;

            // Update page fault count
            pageFaultCount++;
        }

        int frameNumber = getFrameNumFromPageNum(pageNumber);

        // Check if the page is loaded in a frame (frame number != -1)
        if (frameNumber != -1)
        {
            // Update LRU
            searchLRUCache(lruFrame, frameNumber);
        }
        else
        {
            // Get LRU Frame
            frameNumber = getNodeDatum(getTheTailOf(lruFrame));

            // Update LRU
            searchLRUCache(lruFrame, frameNumber);

            // Update frame table
            frameTable[frameNumber].pageNumber = pageNumber;
        }

        // Calculate physical address and retrieve data
        int physicalAddress = frameNumber * FRAME_SIZE + offset;

        if (ram[physicalAddress] == '\0')
        {
            // Load page file from disk
            FILE *processFile = fopen("PROCESS.txt", "r");
            // Seek to the correct page
            fseek(processFile, physicalAddress * 2, SEEK_SET);
            // Read the page directly into the ram array
            fread(&ram[physicalAddress], sizeof(char), PAGE_SIZE, processFile);
            // Close the file
            fclose(processFile);
        }

        char data = ram[physicalAddress];

        // Print logical address, physical address, and data
        printf("Logical Address: %d, Physical Address: %d, Data: %c\n", logicalAddress, physicalAddress, data);
    }

    // Close addresses.txt
    fclose(addressesFile); 

    // Convert page fault count and num pages to floats
    float pfc = pageFaultCount, np = NUM_PAGES;

    // Calculate page fault rate
    double pageFaultRate = (pfc/np) * 100;

    // Print page fault rate
    printf("\nPage-fault rate: %.2f%%\n", pageFaultRate);

    // Free linked list
    // freeThisManyDoublyLinkedNodes(lruFrame, NUM_FRAMES);
    free(lruFrame);

    // Exit
    return 0;
}