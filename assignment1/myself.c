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

// Build a path to a file in the /proc directory
// @param pid The process ID
// @param path The path to the file
char* build_path(int pid, char* path) {
    // Allocate more than enough space for the path
    char* str = malloc(strlen("/proc/") + strlen(path) + 1 + 5);
    sprintf(str, "/proc/%d/%s", pid, path);
    return str;
}

// Main method
int main(void) {
    int processesFound = 0;

    // Iterate over all possible process IDs
    for(int i = 0; i < 32768; i++) {
        // Check to see if the process exists
        char* path = build_path(i, "");

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