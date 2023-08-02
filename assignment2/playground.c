#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    // int burstTime = atoi(argv[2]);

    // Open file from first argument
    FILE *fp;    
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    fp = fopen("input.txt", "r");
    // Iterate over each line of file and add to queue
    while ((read = getline(&line, &len, fp)) != -1) {
        unsigned long burst; // = malloc(sizeof(unsigned long));
        pid_t pid; // = malloc(sizeof(pid_t));
        // Parse things
        sscanf(line, "%d%zu\n", &pid, &burst);

        // Print findings
        printf("PID: %d, Burst: %zu\n", pid, burst);
    }
}