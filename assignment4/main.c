#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "DoublyLinkedList.c"

#define FRAME_SIZE 16
#define NUM_FRAMES 8
#define PAGE_SIZE (FRAME_SIZE)
#define NUM_PAGES (512 / 16) // 32

#define MAX_ADDRESSES 300

typedef struct
{
    bool valid;
    int frameNumber; // -1 if not loaded
} PageTableEntry;

typedef struct
{
    int pageNumber;
} Frame;

int pageFaultCount = 0;

// Track which pages are valid
PageTableEntry pageTable[NUM_PAGES]; // Array with 32 entries

// Track which pages are loaded in which frames
Frame frameTable[NUM_FRAMES]; // Array with 8 entries

// Track faults
int faults[MAX_ADDRESSES];

// RAM
char ram[NUM_FRAMES * FRAME_SIZE];

void readIntoRam(int pageNumber, int physicalAddress, int offset)
{
    // Load page file from disk
    FILE *processFile = fopen("PROCESS.txt", "r");

    // Calculate start of page (x2 because of \n)
    int startOfPage = pageNumber * PAGE_SIZE * 2;

    // Seek to the correct page
    fseek(processFile, startOfPage, SEEK_SET);

    int physicalAddrStart = physicalAddress - offset;

    // Read page into ram, skipping \n
    for (int i = 0; i < PAGE_SIZE; i++)
    {
        char c = fgetc(processFile);
        if (c == '\n')
        {
            i--;
            continue;
        }
        ram[physicalAddrStart + i] = c;
    }

    // Close the file
    fclose(processFile);
}

// Search through a doubly linked list LRU-style:
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

int main()
{
    // Initialize page table
    struct DoublyLinkedList *lruFrame = malloc(sizeof(DoublyLinkedList) * (NUM_FRAMES * sizeof(DoublyLinkedNode)));
    addThisManyEmptyNodes(lruFrame, NUM_FRAMES);

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
        pageTable[i].frameNumber = -1;
    }

    // Initialize frame table with all entries as empty
    for (int i = 0; i < NUM_FRAMES; i++)
    {
        frameTable[i].pageNumber = -1;

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

    float totalAddresses = 0;

    printf("Logical Address   Physical Address  Data\n");

    // Read addresses.txt line by line
    while (fscanf(addressesFile, "%d\n", &logicalAddress) != EOF)
    {
        // Calculate page number and offset
        int pageNumber = logicalAddress / PAGE_SIZE;
        int offset = logicalAddress % PAGE_SIZE;

        // Get frame number from page table (if it is -1, page is unloaded)
        int frameNumber = pageTable[pageNumber].frameNumber;

        // Calculate physical address
        int physicalAddress = frameNumber * FRAME_SIZE + offset;

        // Check if the page is loaded in a frame (frame number != -1)
        if (frameNumber != -1)
        {
            // Update LRU
            searchLRUCache(lruFrame, frameNumber);
        }
        else
        {
            // Add to page fault list
            faults[pageFaultCount] = pageNumber;

            // Update page fault count
            pageFaultCount++;

            // Get least-recently-used frame
            frameNumber = getNodeDatum(getTheTailOf(lruFrame));

            // Get page number that's at that current frame
            int pageNumToInvalidate = frameTable[frameNumber].pageNumber;

            // Invalidate least-recently-used page
            pageTable[pageNumToInvalidate].valid = false;
            pageTable[pageNumToInvalidate].frameNumber = -1;

            // Update least-recently-used
            searchLRUCache(lruFrame, frameNumber);

            // Re-calculate physical address
            physicalAddress = frameNumber * FRAME_SIZE + offset;

            // Load page into RAM
            readIntoRam(pageNumber, physicalAddress, offset);

            // Mark page as valid
            pageTable[pageNumber].valid = true;

            // Update frame number in page table, as page is now loaded in frame
            pageTable[pageNumber].frameNumber = frameNumber;

            // Update frame number in frame table
            frameTable[frameNumber].pageNumber = pageNumber;
        }

        // Data is now in RAM
        char data = ram[physicalAddress];

        char *final = malloc(sizeof(char) * 3);
        final[0] = data;
        final[1] = '\0';
        final[2] = '\0';
        if (data == '\n')
        {
            final[0] = '\\';
            final[1] = 'n';
        }

        // Print logical address, physical address, and data
        printf("%-17d %-17d %s \n", logicalAddress, physicalAddress, final);

        // Free final
        free(final);

        // Increment total addresses
        totalAddresses++;
    }

    // Close addresses.txt
    fclose(addressesFile);

    // Convert page fault count and num pages to floats
    float pfc = pageFaultCount;

    // Calculate page fault rate
    double pageFaultRate = (pfc / totalAddresses) * 100;

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