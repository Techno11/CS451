#include <sys/stat.h>   // stat
#include <stdbool.h>    // bool type
#include <stdio.h>      // printf 
#include <string.h>     // strcat
#include <time.h>       // ctime
#include <stdlib.h>     // malloc
#include <pwd.h>        // getpwuid

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

    // Print the header
    printf("%-5s %-5s %-5s %-5s %-5s %-5s %-5s %-5s %-5s\n", "PID", "UID", "PPID", "LWP", "NLWP", "STIME", "STAT", "TIME", "CMD");

    // Iterate over all possible process IDs
    // 32768
    for(int i = 0; i < 2; i++) {
        // Check to see if the process exists
        char* path = build_path(i, "");

        // Check to see if the file exists
        if(file_exists(path)) {
            // Stat the process
            struct stat st;
            stat(path, &st);

            // Process UID
            struct passwd *pwd = getpwuid(st.st_uid);
            char* UID = pwd->pw_name;

            // Process PPID

            // Process LWP

            // Process NLWP

            // Process STIME
            char* STIME = malloc(5);
            strftime(STIME, 100, "%H:%M", localtime(&st.st_atime));

            // Process STAT

            // Process TIME

            // Process CMD

            // Print that it exists
            printf("%-5d %-5s %-5d %-5d %-5d %-5s %-5s %-5s %-5s\n", i, UID, 0, 0, 0, STIME, "S", "0:00", "CMD");
            processesFound++;
            // TODO: Read and print the processes statistics
            
        }
    }

    // Print total # processes found
    printf("Found %d processes\n", processesFound);

    // Exit
    return 0;
}