//
// Created by bsting on 7/12/23.
//
#include <sys/stat.h> // stat (import struct def. of stat)
#include <stdbool.h>  // bool type
#include <stdio.h>    // printf
#include <stdlib.h>   //for alloc functions
#include <string.h>   // strcat
#include <time.h>
#include <pwd.h>
#include <unistd.h> // sysconf

// Finds the number of characters on a line in a file:
//(use this with fgets() to properly declare max char size param)
int amtOfCharOnLine(char *filepath, int lineNum)
{
    int totalChars = 0;
    int currLineNum = 0;
    char currentChar;

    FILE *file = fopen(filepath, "r");
    currentChar = getc(file);

    // First, locate correct line to count on
    while (currentChar != EOF)
    {
        if (currentChar == '\n')
        {
            currLineNum++;
        }
        if (currLineNum == lineNum)
        {
            break;
        }
        currentChar = getc(file);
    }

    // Then count each character on correct line until next line or EOF is encountered:
    while ((currentChar != EOF) && (currentChar != '\n'))
    {
        currentChar = getc(file);
        totalChars++;
    }
    fclose(file);
    return totalChars;
}

// Finds the total lines in a file
int findFileLines(char *filepath)
{
    // Initialize values for total file line count and character tracker
    int totalLines = 0;
    char currentChar;

    // Begin with opening file and getting the first character
    FILE *file = fopen(filepath, "r");
    currentChar = getc(file);

    // While we haven't reached the end of file, count the # of newlines
    while (currentChar != EOF)
    {
        if (currentChar == '\n')
        {
            totalLines = totalLines + 1;
        }
        currentChar = getc(file);
    }

    fclose(file);

    return totalLines + 1; // Return the total number of file lines
    //(e.g. 0 1 2 = ...\n...\n...EOF = total lines are 3
}

// Get the number of digits in an integer (useful for int-to-str)
int countIntDigits(long passedInt)
{
    // Filter out single digits:
    passedInt = passedInt / 10;
    int count = 1; // digit tracker (even 0 has 1 digit)

    while (passedInt != 0) // while there's still digits left (no cutoff)
    {
        passedInt = passedInt / 10; // cut off next decimal place
        count++;                    // we moved over, so that's a digit
    }
    return count;
}

// Build a path to a file in the /proc directory
// @param pid The process ID
// @param path The path to the file
char *buildPath(int pid, char *path)
{
    // Allocate more than enough space for the path
    char *str = malloc(strlen("/proc/") + strlen(path) + 1 + 5); // 5 for "/proc/", 1 for '\0'
    sprintf(str, "/proc/%d/%s", pid, path);                      // write the formatted string to buffer pointed by str
    return str;                                                  // return the fully allocated and fully formatted/written path
}

// Get process TIME value
// @param pid The process ID
char *getProcTime(int pid)
{
    // stat file breakdown:
    // https://man7.org/linux/man-pages/man5/proc.5.html#:~:text=ptrace(2).-,/proc/pid/stat,-Status%20information%20about

    // Get the path of the stat file
    char *statPath = buildPath(pid, "/stat");
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

    // Calculate num seconds
    unsigned long seconds = (total - (hours * 360000) - (minutes * 6000)) / 100;

    char *prettyTime = malloc(10);
    sprintf(prettyTime, "%lu:%02lu:%02lu", hours, minutes, seconds);

    // Clean up
    free(a);
    free(utime);
    free(stime);
    free(statPath);
    fclose(statFile);

    return prettyTime;
}

char *getCmd(int pid)
{
    // Build path to cmdline file
    char *cmdlineFile = buildPath(pid, "cmdline");
    // Open the cmdline file
    FILE *cmdlinePtr = fopen(cmdlineFile, "rb");

    // Track buffer size
    int size = 0;

    // Track current and last character
    unsigned char c = 0;
    unsigned char last = 0;

    char *buffer = malloc(1);
    while (true)
    {
        // Read next character
        c = fgetc(cmdlinePtr);

        // Check for EOF
        if (c == 255 && last == 255 && c != '\0')
        {
            break;
        }

        // Check for null terminator or nbsp
        if (c == '\0' || c == 255)
        {
            // Extend buffer by 1, put space at end of buffer
            buffer = realloc(buffer, size + 1);
            buffer[size] = ' ';
        }
        else
        {
            // Extend buffer by 1, put char at end of buffer
            buffer = realloc(buffer, size + 1);
            buffer[size] = c;
        }

        // Increase size and set last char
        size++;
        last = c;
    }

    buffer[size] = 0;

    // Clean up
    free(cmdlineFile);
    fclose(cmdlinePtr);

    return buffer;
}

// Check to see if a file exists
// @param filename The name of the file to check
bool fileExists(char *filepath)
{
    struct stat buffer;                    // reference to stat's buffer (will fill next)
    return (stat(filepath, &buffer) == 0); // returns 0 on stat success, else -1 + err
}

// Main method
int main(void)
{
    int processesFound = 0;

    // Print the header with padding
    printf("%-5s %-16s %-5s %-5s %-5s %-5s %-5s %-10s %-5s\n", "PID", "UID", "PPID", "LWP", "NLWP", "STIME", "STAT", "TIME", "CMD");

    // Iterate over all possible process IDs (PIDs)
    // TODO: Make this more efficient by reading files in the /proc directory

    // 32768
    for (int i = 0; i < 5; i++)
    {
        // Build the path to the PID folder
        char *path = buildPath(i, "");

        // Check to see if the file exists
        if (fileExists(path))
        {
            // IT'S A VALID PID:

            // Get status file inside PID folder; it has good info we need
            char *statusFile = buildPath(i, "status");
            int numOfLines = findFileLines(statusFile);
            FILE *statusPtr = fopen(statusFile, "r");

            struct stat st;
            stat(path, &st);

            // Process UID
            struct passwd *pwd = getpwuid(st.st_uid);
            char *UID = pwd->pw_name;

            // Process PID
            int numLength = countIntDigits(i);
            // char pid[numLength + 1 + 1]; // +1 for (-) and +1 for '\0'
            char *strPid;
            strPid = (char *)malloc((sizeof(char) * numLength) + 1 + 1); // +1 for (-) and +1 for '\0'
            sprintf(strPid, "%d", i);

            // Process PPID
            // TODO: Read status file (opened above) and find line where 'PPid' is at the start
            // TODO: Then, extract the 'PPid' value and store it in a char string here for later
            // (use amtOfCharOnLine() to find total number of characters on each file line)
            int charsOnLine = 0;
            for (int lineIndex = 0; lineIndex < numOfLines; lineIndex++)
            {
                charsOnLine = amtOfCharOnLine(statusFile, lineIndex);
                char *fileLine;
                fileLine = (char *)malloc((sizeof(char) * charsOnLine) + 1 + 1); //+1 for '\0' and + 1 for '\n' in case
                fgets(fileLine, charsOnLine, statusPtr);
                // TODO: Check for the PPid!
            }

            // Process LWP

            // Process NLWP

            // Process STIME
            char *STIME = malloc(5);
            strftime(STIME, 100, "%H:%M", localtime(&st.st_atime));

            // Process STAT

            // Process TIME
            char *prettyTime = getProcTime(i);

            // Process CMD
            char *cmd = getCmd(i);

            // Print the status row with padding
            printf("%-5d %-16s %-5d %-5d %-5d %-5s %-5s %-10s %-5s\n", i, UID, 0, 0, 0, STIME, "S", prettyTime, cmd);

            // Free up memory (anything that was malloc'd above should be freed here before the next loop iteration)
            free(prettyTime);
            free(cmd);

            // For comparison purposes
            processesFound++;
        }
    }

    // Print total # processes found
    printf("Found %d processes\n", processesFound);

    // Exit
    return 0;
}