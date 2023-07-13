#include <sys/stat.h> // stat
#include <stdbool.h>  // bool type
#include <stdio.h>    // printf
#include <string.h>   // strcat
#include <time.h>     // ctime
#include <stdlib.h>   // malloc
#include <pwd.h>      // getpwuid
#include <unistd.h>   // sysconf

// Check to see if a file exists
// @param filename The name of the file to check
bool file_exists(char *filename)
{
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

// Build a path to a file in the /proc directory
// @param pid The process ID
// @param path The path to the file
char *build_path(int pid, char *path)
{
    // Allocate more than enough space for the path
    char *str = malloc(strlen("/proc/") + strlen(path) + 1 + 5);
    sprintf(str, "/proc/%d/%s", pid, path);
    return str;
}

char *get_proc_time(int pid)
{
    // stat file breakdown:
    // https://man7.org/linux/man-pages/man5/proc.5.html#:~:text=ptrace(2).-,/proc/pid/stat,-Status%20information%20about

    // Get the path of the stat file
    char *statPath = build_path(pid, "/stat");
    // Open the stat file
    FILE *statFile = fopen(statPath, "r");
    // Create dummy variables that we don't care about
    char *a = malloc(128);

    // Get the utime from the stat file
    unsigned long int *utime = malloc(64);
    unsigned long int *stime = malloc(64);
    fscanf(statFile, "%s %s %s %s %s %s %s %s %s %s %s %s %s %lu %lu %s", a, a, a, a, a, a, a, a, a, a, a, a, a, utime, stime, a);

    // Get the total time
    unsigned long total = (*utime / sysconf(_SC_CLK_TCK)) + (*stime / sysconf(_SC_CLK_TCK));

    // Calculate num hours
    unsigned long hours = total / 360000;

    // Calculate num minutes
    unsigned long minutes = (total - (hours * 360000)) / 6000;

    char *prettyTime = malloc(8);
    sprintf(prettyTime, "%lu:%02lu", hours, minutes);

    // Clean up
    free(a);
    free(utime);
    free(stime);
    free(statPath);
    fclose(statFile);

    return prettyTime;
}

// Main method
int main(void)
{
    int processesFound = 0;

    // Print the header
    printf("%-5s %-5s %-5s %-5s %-5s %-5s %-5s %-5s %-5s\n", "PID", "UID", "PPID", "LWP", "NLWP", "STIME", "STAT", "TIME", "CMD");

    // Iterate over all possible process IDs
    // 32768
    for (int i = 0; i < 2; i++)
    {
        // Check to see if the process exists
        char *path = build_path(i, "");

        // Check to see if the file exists
        if (file_exists(path))
        {
            // Stat the process
            struct stat st;
            stat(path, &st);

            // Process UID
            struct passwd *pwd = getpwuid(st.st_uid);
            char *UID = pwd->pw_name;

            // Process PPID

            // Process LWP

            // Process NLWP

            // Process STIME
            char* STIME = malloc(5);
            strftime(STIME, 100, "%H:%M", localtime(&st.st_atime));

            // Process STAT

            // Process TIME
            char *prettyTime = get_proc_time(i);

            // Process CMD

            // Print that it exists
            printf("%-5d %-5s %-5d %-5d %-5d %-5s %-5s %-5s %-5s\n", i, UID, 0, 0, 0, STIME, "S", prettyTime, "CMD");

            // Free up memory
            free(prettyTime);

            processesFound++;
            // TODO: Read and print the processes statistics
        }

        // Clean up
        free(path);
    }

    // Print total # processes found
    printf("Found %d processes\n", processesFound);

    // Exit
    return 0;
}