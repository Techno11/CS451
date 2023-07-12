#include <sys/stat.h>   // stat
#include <stdbool.h>    // bool type
#include <stdio.h>    // printf 
#include <string.h>   // strcat

bool file_exists (char *filename) {
  struct stat   buffer;   
  return (stat (filename, &buffer) == 0);
}

char* itoa(int val, int base){
	
	static char buf[32] = {0};
	
	int i = 30;
	
	for(; val && i ; --i, val /= base)
	
		buf[i] = "0123456789abcdef"[val % base];
	
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
        }
    }

    printf("Found %d processes\n", processesFound);

    return 0;
}