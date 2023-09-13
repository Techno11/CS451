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
    int frameNumber;
} PageTableEntry;

int pageFaultCount = 0;
int currentFrame = 0;

PageTableEntry pageTable[NUM_PAGES]; // Array with 32 entries

// Search through a doubly linked list LRU-style:
int searchLRUCache(DoublyLinkedList* LRUStack, int targetDatum)
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

void LRUOperation(int array[], DoublyLinkedList* LRUList, int amountOfElements)
{
    for (int index = 0; index < amountOfElements; index++)
    {
        searchLRUCache(LRUList, array[index]);
        displayDoublyLinkedList(LRUList);
    }
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
    //struct DoublyLinkedList* pageTableLRU = malloc(sizeof(DoublyLinkedList) * (NUM_PAGES * sizeof(DoublyLinkedNode)));
    struct DoublyLinkedList* pageTableLRU = malloc((sizeof(DoublyLinkedList)) * (NUM_PAGES));
    addThisManyEmptyNodes(pageTableLRU, NUM_PAGES);

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

        if (logicalAddress == 555)
        {
            break;
        }

        // Calculate page number and offset
        int pageNumber = logicalAddress / PAGE_SIZE;
        int offset = logicalAddress % PAGE_SIZE;

        // Check if the page is valid in the page table
        if (pageTable[pageNumber].valid == false)
        {
            // Update page table
            pageTable[pageNumber].valid = true;
            pageTable[pageNumber].frameNumber = currentFrame;

            // Update LRU with the page number we just accessed
            searchLRUCache(pageTableLRU, pageNumber);

            // Update current frame
            currentFrame = (currentFrame + 1) % NUM_FRAMES;

            pageFaultCount++;
        }
        else 
        {
            // Get the LRU page number
            pageNumber = (getTheTailOf(pageTableLRU))->datum;

            // Update page table
            pageTable[pageNumber].frameNumber = currentFrame;

            // Update current frame
            currentFrame = (currentFrame + 1) % NUM_FRAMES;

            // Update LRU with the page number we're accessing
            searchLRUCache(pageTableLRU, pageNumber);
        }

        // Calculate physical address and retrieve data
        int physicalAddress = pageTable[pageNumber].frameNumber * FRAME_SIZE + offset;

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

    printf("I've exited the loop, boss\n");

    // Close addresses.txt
    fclose(addressesFile);

    // Free linked list
    //freeThisManyDoublyLinkedNodes(pageTableLRU, NUM_PAGES);

    //free(pageTableLRU->head);
    //free(pageTableLRU->temp);
    //free(pageTableLRU->tail);

    //free(pageTableLRU);
    freeListV2(pageTableLRU->head);

    // Print page-fault rate
    float pageFaultRate = (float)pageFaultCount / NUM_PAGES * 100;
    printf("\nPage-fault rate: %.2f%%\n", pageFaultRate);

    // Exit
    return 0;
}