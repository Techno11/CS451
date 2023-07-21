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
#include <dirent.h> // opendir, readdir, closedir

// Finds the number of characters on a line in a file:
//(use this with fgets() to properly declare max char size param)
size_t amtOfCharOnLine(char *filepath, int lineNum)
{
    size_t totalChars;
    int currLineNum = 0;
    char currentChar;

    FILE *file = fopen(filepath, "r");
    currentChar = getc(file);

    // First, locate correct line to count on
    while (currentChar != EOF)
    {
        if (currLineNum == lineNum)
        {
            break;
        }
        if (currentChar == '\n')
        {
            currLineNum++;
        }
        currentChar = getc(file);
    }

    // Then count each character on correct line until next line or EOF is encountered:
    /*while ((currentChar != EOF) && (currentChar != '\n'))
    {
        currentChar = getc(file);
        totalChars++;
    }*/
    char* buffer;
    size_t bufferSize = 32;
    totalChars = getline(&buffer, &bufferSize, file); //will return total characters on the line (include \n)
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

void findAndReplaceChar(char toFind, char toReplaceWith, char* str)
{
    for (int ind = 0; str[ind] != '\0'; ind++)
    {
        if (str[ind] == toFind)
        {
            str[ind] = toReplaceWith;
            break; //will only replace first instance
        }
    }
}

// Build a path to a file in the /proc/pid directory
// @param pid The process ID
// @param path The path to the file
char *buildPidPath(char *basePath, int pid, char *path)
{
    // Allocate more than enough space for the path
    char *str = malloc(strlen(basePath) + strlen(path) + 1 + 5); // 5 for max pid length, 1 for '/'
    sprintf(str, "%s/%d/%s", basePath, pid, path);               // write the formatted string to buffer pointed by str
    return str;                                                  // return the fully allocated and fully formatted/written path
}

// Build a path to a file in the /proc directory
// @param pid The process ID
// @param path The path to the file
char *buildProcPath(char *basePath, char *path)
{
    // Allocate more than enough space for the path
    char *str = malloc(strlen(basePath) + strlen(path) + 1); // 1 for '/'
    sprintf(str, "%s/%s", basePath, path);               // write the formatted string to buffer pointed by str
    return str;                                                  // return the fully allocated and fully formatted/written path
}

// Get process TIME value
// @param pid The process ID
char *getProcTime(unsigned long int *utime, unsigned long int *stime)
{
    // Get the total time
    unsigned long total = (*utime / sysconf(_SC_CLK_TCK)) + (*stime / sysconf(_SC_CLK_TCK));

    // Calculate num hours
    unsigned long hours = total / 3600;

    // Calculate num minutes
    unsigned long minutes = (total - (hours * 3600)) / 60;

    // Calculate num seconds
    unsigned long seconds = (total - (hours * 3600) - (minutes * 60));

    char *prettyTime = malloc(10);
    sprintf(prettyTime, "%lu:%02lu:%02lu", hours, minutes, seconds);

    return prettyTime;
}

char* getPrettySTIME(unsigned long long totalSTIMESeconds)
{
    unsigned long hours = totalSTIMESeconds / 3600;
    unsigned long minutes = (totalSTIMESeconds - (hours * 3600)) / 60;
    unsigned long days = hours / 24;
    unsigned long months = days / 30;
    unsigned long years = months / 12;
}

unsigned long long getStartTime(char* procPath, unsigned long long *startTime)
{

    unsigned long long *totalStartTime = malloc(sizeof(unsigned long long) + 1);
    //Now in seconds:
    *totalStartTime = (*startTime / sysconf(_SC_CLK_TCK)); // seconds

    // Get the path of the stat file
    char *procStatPath = buildProcPath(procPath, "/stat");
    // Open the stat file
    FILE *statFile = fopen(procStatPath, "r"); // like $ cat /proc/stat
    char* buffer;
    size_t buffSize = 128;
    buffer = (char *)malloc(buffSize * sizeof(char));

    int totalLines = findFileLines(procStatPath);
    int currentLine;
    char* substringPtr;

    for (currentLine = 0; currentLine < totalLines; currentLine++)
    {
        getline(&buffer, &buffSize, statFile);
        substringPtr = strstr(buffer, "btime");

        //if (strstr(buffer, "btime") != NULL)
        if (substringPtr)
        {
            //char* realValuePtr = substringPtr + 6;
            char* realValueStr = (char*)malloc(sizeof(char) * (strlen(buffer)+1));
            strcpy(realValueStr, substringPtr+6);
            char* endPtr;
            unsigned long realValue = strtoul(realValueStr, &endPtr, 10);
            unsigned long long result = *totalStartTime + realValue;
            return result;
            // --THIS IS USING NEW ALGORITHM FOR GETTING STIME--
            //  1. Get start_time of individual process after system boot
            //  2. Find the system boot time in seconds
            //  3. Add start_time (seconds) to system boot time (btime)
        }
    }


    // TODO: use /proc/uptime and ADD to totalStartTime to = STIME in raw seconds
    //  (/proc/uptime is in seconds; read or extract the FIRST value in uptime)
    // see man7.org/linux/man-pages/man5/proc.5.html and CTRL+F 'uptime'
    // Format STIME into what ps does from there after seconds are calculated with uptime

    //return totalStartTime; //WILL BE 0 MOST OF TIME SINCE SMALL CLOCK TICKS
    // Minutes:

}

char* getStartTimeSoren(char* basePath, unsigned long long *statStartTime)
{
    // Build path to uptime file
    char *statFile = buildProcPath("/proc", "stat");
    printf("%s\n", statFile);
    // Open the cmdline file
    FILE *statPtr = fopen("/proc/stat", "rb");
    // Variable to read from file
    char* a = malloc(20);
    char* b = malloc(200);
    unsigned long long *bootTime = malloc(sizeof(unsigned long long) + 1);
    // Read first line of uptime file
    fscanf(statPtr, "%sbtime %s\n", a, b, a);
    // Close uptime file
    fclose(statPtr);
    free(a);

    printf("%s\n", b);

    // printf("%llu\n", *bootTime);

    // Calculate seconds from stat uptime
    unsigned long long *statUptime = malloc(sizeof(unsigned long long) + 1);
    *statUptime = *statStartTime / sysconf(_SC_CLK_TCK);

    // Calculate seconds from uptime file
    unsigned long long *uptime = malloc(sizeof(unsigned long long) + 1);
    *uptime = *bootTime + *statUptime;

    // Convert startTime to a string
    char *startTimeStr = malloc(100);
    strftime(startTimeStr, 100, "%H:%M", localtime(&uptime));

    return startTimeStr;
}

char *getCmd(char *basePath, int pid)
{
    // Build path to cmdline file
    char *cmdlineFile = buildPidPath(basePath, pid, "cmdline");
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

    buffer[size] = '\0';

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

void processPid(char *basePath, int pid, int parentPid, char *parentSTIME)
{
    // Build the path to the PID folder
    char *path = buildPidPath(basePath, pid, "");

    // Check to see if the file exists
    if (fileExists(path))
    {
        // IT'S A VALID PID:

        /** READ STAT FILE **/
        // stat file breakdown:
        // https://man7.org/linux/man-pages/man5/proc.5.html#:~:text=ptrace(2).-,/proc/pid/stat,-Status%20information%20about

        // Create dummy variables that we don't care about
        char *a = malloc(128);

        // Get the path of the stat file
        char *statPath = buildPidPath(basePath, pid, "/stat");
        // Open the stat file
        FILE *statFile = fopen(statPath, "r");

        // Get the utime and stime from the stat file to use to calculate TIME value
        unsigned long int *utime = malloc(64);
        unsigned long int *stime = malloc(64);

        // Get parent process ID (PPID)
        int *PPID = malloc(32);

        // Get number of threads (NLWP)
        unsigned long int *NLWP = malloc(64);

        // Process comm name should be allocated (numOfChars * 1) + 1 [1 byte per char]
        // According to man, it is truncated to 16 chars, including null byte (so 18 for ())
        char *comm = malloc(18);

        // Get start time (Either YY in years (not started in same year), "MmmDD" if it was not
        // started the same day, or "HH:MM" otherwise.) IF-ELSEIF-ELSE
        // Will be scanned in as unsigned long-long clock ticks:
        // (22) starttime %llu, the time the process started after system boot.
        unsigned long long *startTime = malloc(sizeof(unsigned long long) + 1);


        // Parse the stat file as documented, using "a" for the values we don't care about, and capturing the values we do care about in the variables we created above
        fscanf(statFile, "%s %s %c %d %s %s %s %s %s %s %s %s %s %lu %lu %s %s %s %s %lu %s %llu %s", a, comm, a, PPID, a, a, a, a, a, a, a, a, a, utime, stime, a, a, a, a, NLWP, a, startTime, a);

        // Clean up
        free(a);
        free(statPath);
        fclose(statFile);
        /** Done Reading STAT File **/

        // STAT the PID directory, allows us to get the UID of the process
        struct stat st;
        stat(path, &st);

        // Process UID
        struct passwd *pwd = getpwuid(st.st_uid);   // Get the passwd struct for the UID (contains username)
        char *UID = pwd->pw_name;                   // Fetch the username from the passwd struct

        // Process PID
        int printPid = pid;                         // PID to print, defaults to current PID
        if (parentPid > 0)                          // If the parent PID is greater than 0, we are in a child process, and the printed PID is the parent PID
        {
            printPid = parentPid;
        }

        // Process LWP                              // LWP is always the same as the current PID
        int LWP = pid;

        // Process STIME
        char *STIME = malloc(20);                    // STIME to print, defaults to current STIME
        if (parentPid > 0)                          // If the parent PID is greater than 0, we are in a child process, and the printed STIME is the parent STIME
        {
            STIME = parentSTIME;
        }
        else                                        // If the parent PID is 0, we are in the parent process, and the printed STIME is the STIME from the STAT
        {
            STIME = getStartTimeSoren(basePath, startTime);
        }

        // Process TIME
        char *prettyTime = getProcTime(utime, stime);   // Get the pretty TIME string

        // Process CMD
        char *cmd = getCmd(basePath, pid);              // Calculate the CMD string
        // If CMD string is blank, then use the process' comm name:
        if (cmd[0] == ' ')
        {
            cmd = comm;
            findAndReplaceChar('(', '[', cmd);
            // TODO: Change find and replace last ) to start from end of string (so doesn't replace wrong guy)
            findAndReplaceChar(')', ']', cmd);
        }

        // Print the status row with padding
        printf("%-16s %-5d %-5d %-5d %-5lu %-5s %-10s %-5s\n", UID, printPid, *PPID, LWP, *NLWP, STIME, prettyTime, cmd);

        // Find child processes
        if (parentPid == 0)
        {
            // Build path to parent's child processes folder
            char *childPath = buildPidPath(basePath, pid, "task");
            for (int i = 0; i < 32768; i++)
            {
                // If the child process we found is not the parent process, then process it
                if (i != pid)
                {
                    // Recursively Process a child PID
                    processPid(childPath, i, pid, STIME);
                }
            }
        }

        // Free up memory (anything that was malloc'd above should be freed here before the next loop iteration)
        free(prettyTime);
        free(cmd);
        free(utime);
        free(stime);
        free(NLWP);

        // Only allow parent processes to free STIME, otherwise it will fail for parents with more than one child
        if (parentPid == 0)
        {
            free(STIME);
        }
    }
}

// Main method
int main(void)
{
    // Print the header with padding
    printf("%-16s %-5s %-5s %-5s %-5s %-5s %-10s %-5s\n", "UID", "PID", "PPID", "LWP", "NLWP", "STIME", "TIME", "CMD");

    // Iterate over all possible process IDs (PIDs)
    // TODO: Make this more efficient by reading files in the /proc directory

    // 32768
    for (int i = 0; i < 5; i++)
    {
        // Process a parent PID
        processPid("/proc", i, 0, "");
    }

    // Exit
    return 0;
}