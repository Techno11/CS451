#include <sys/stat.h>   // stat
#include <stdbool.h>    // bool type
#include <stdio.h>    // printf 
#include <string.h>   // strcat

// Check to see if a file exists
// @param filename The name of the file to check
bool file_exists (char *filename) {
  struct stat   buffer;   
  return (stat (filename, &buffer) == 0);
}

// Convert an integer to a string
// @param val The integer to convert
// @param base The base to convert to (radix)
char* itoa(int val, int base){
    // Create a static buffer
	static char buf[32] = {0};
    // Initialize the index
	int i = 30;
    // Convert the integer to a string
	for(; val && i ; --i, val /= base) {
		buf[i] = "0123456789abcdef"[val % base];
    }
	// Do some fancy pointer arithmetic to return the string
	return &buf[i+1];
}

// Main method
int main(void) {
    int processesFound = 0;

    // Iterate over all possible process IDs
    for(int i = 0; i < 32768; i++) {
        // Check to see if the process exists
        char path[] = "/proc/";
        strcat(path, itoa(i, 10));

        // Check to see if the file exists
        if(file_exists(path)) {
            // Print that it exists
            printf("Process %d exists\n", i);
            processesFound++;
            // TODO: Read and print the processes statistics
        }
    }

    // Print total # processes found
    printf("Found %d processes\n", processesFound);

    // Exit
    return 0;
}