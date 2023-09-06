#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

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
    time_t lastAccessed;
} PageTableEntry;

int pageFaultCount = 0;
int currentFrame = 0;

PageTableEntry pageTable[NUM_PAGES]; // Array with 32 entries

int main()
{
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
        // REMOVE THIS, FOR TESTING
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
            pageTable[pageNumber].lastAccessed = time(NULL);

            // Update current frame
            currentFrame = (currentFrame + 1) % NUM_FRAMES;

            pageFaultCount++;
        }
        else 
        {
            // LRU ALROGITHM HERE!

            // time_t oldest time found
            // int oldest time index
            // bool found existing frame number

            // iterate over all page table entries

            // update the oldest index found IF we didn't find an entry with the same frame number

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

    // Close addresses.txt
    fclose(addressesFile);

    // Print page-fault rate
    float pageFaultRate = (float)pageFaultCount / NUM_PAGES * 100;
    printf("\nPage-fault rate: %.2f%%\n", pageFaultRate);

    // Exit
    return 0;
}