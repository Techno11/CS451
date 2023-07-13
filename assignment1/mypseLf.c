//
// Created by bsting on 7/12/23.
//
#include <sys/stat.h>   // stat (import struct def. of stat)
#include <stdbool.h>    // bool type
#include <stdio.h>    // printf
#include <stdlib.h> //for alloc functions
#include <string.h>   // strcat
#include <time.h>
#include <pwd.h>

//Finds the number of characters on a line in a file:
//(use this with fgets() to properly declare max char size param)
int amtOfCharOnLine(char* filepath, int lineNum)
{
    int totalChars = 0;
    int currLineNum = 0;
    char currentChar;

    FILE* file = fopen(filepath, "r");
    currentChar = getc(file);

    // First, locate correct line to count on
    while (currentChar!= EOF)
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

    //Then count each character on correct line until next line or EOF is encountered:
    while ((currentChar != EOF) && (currentChar != '\n'))
    {
        currentChar = getc(file);
        totalChars++;
    }
    fclose(file);
    return totalChars;
}

//Finds the total lines in a file
int findFileLines(char *filepath)
{
    //Initialize values for total file line count and character tracker
    int totalLines = 0;
    char currentChar;

    //Begin with opening file and getting the first character
    FILE * file = fopen(filepath, "r");
    currentChar = getc(file);

    //While we haven't reached the end of file, count the # of newlines
    while (currentChar != EOF)
    {
        if (currentChar == '\n')
        {
            totalLines = totalLines + 1;
        }
        currentChar = getc(file);
    }

    fclose(file);

    return totalLines + 1; //Return the total number of file lines
    //(e.g. 0 1 2 = ...\n...\n...EOF = total lines are 3
}

// Get the number of digits in an integer (useful for int-to-str)
int countIntDigits (long passedInt)
{
    //Filter out single digits:
    passedInt = passedInt / 10;
    int count = 1; //digit tracker (even 0 has 1 digit)

    while (passedInt != 0) //while there's still digits left (no cutoff)
    {
        passedInt = passedInt / 10; //cut off next decimal place
        count++; //we moved over, so that's a digit
    }
    return count;
}

// Check to see if a file exists
// @param filename The name of the file to check
bool file_exists (char *filepath) {
    struct stat buffer; //reference to stat's buffer (will fill next)
    return (stat (filepath, &buffer) == 0); // returns 0 on stat success, else -1 + err
}

// Convert an integer to a string (ERR: causes stack smash abort)
// @param val The integer to convert
// @param base The base to convert to (radix)
/*char* itoa(int val, int base){
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
}*/

// Build a path to a file in the /proc directory
// @param pid The process ID
// @param path The path to the file
char* build_path(int pid, char* path) {
    // Allocate more than enough space for the path
    char* str = malloc(strlen("/proc/") + strlen(path) + 1 + 5); //5 for "/proc/", 1 for '\0'
    sprintf(str, "/proc/%d/%s", pid, path); //write the formatted string to buffer pointed by str
    return str; //return the fully allocated and fully formatted/written path
}

// Main method
int main(void) {
    int processesFound = 0;

    // Print the header
    printf("%-5s %-5s %-5s %-5s %-5s %-5s %-5s %-5s %-5s\n", "PID", "UID", "PPID", "LWP", "NLWP", "STIME", "STAT", "TIME", "CMD");

    // Iterate over all possible process IDs (PIDs)
    for(int i = 0; i < 32768; i++) {
        // Check to see if the process exists
        //char path[] = "/proc/";
        //strcat(path, itoa(i, 10));
        char *path = build_path(i, "");

        // Check to see if the file exists
        if(file_exists(path))
        {
            // IT'S A VALID PID:

            // Get status file inside PID folder; it has good info we need
            char* statusFile = build_path(i, "status");
            int numOfLines = findFileLines(statusFile);
            FILE* statusPtr = fopen(statusFile, "r");

            struct stat st;
            stat(path, &st);

            // Process UID
            struct passwd *pwd = getpwuid(st.st_uid);
            char* UID = pwd->pw_name;

            // Process PID
            int numLength = countIntDigits(i);
            //char pid[numLength + 1 + 1]; // +1 for (-) and +1 for '\0'
            char* strPid;
            strPid = (char*)malloc((sizeof(char) * numLength) + 1 + 1); // +1 for (-) and +1 for '\0'
            sprintf(strPid, "%d", i);

            // Process PPID
            // TODO: Read status file (opened above) and find line where 'PPid' is at the start
            // TODO: Then, extract the 'PPid' value and store it in a char string here for later
            // (use amtOfCharOnLine() to find total number of characters on each file line)
            int charsOnLine = 0;
            for (int lineIndex = 0; lineIndex < numOfLines; lineIndex++)
            {
                charsOnLine = amtOfCharOnLine(statusFile, lineIndex);
                char* fileLine;
                fileLine = (char*)malloc((sizeof(char)*charsOnLine) + 1 + 1); //+1 for '\0' and + 1 for '\n' in case
                fgets(fileLine, charsOnLine, statusPtr);
                // TODO: Check for the PPid!
            }

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

            // Print that it exists
            //printf("Process %d exists\n", i);
            processesFound++;
            // TODO: Read and print the processes statistics
        }
    }

    // Print total # processes found
    printf("Found %d processes\n", processesFound);

    // Exit
    return 0;
}