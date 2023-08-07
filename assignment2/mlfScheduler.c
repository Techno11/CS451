/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 2
        Date of Submission: 7/24/2023
        Name of this file: mlfScheduler.c
        Short Description of contents:
            This program runs a scheduler of child processes that process prime numbers. Processes first executed for a standard amount of time, then paused, then resumed for the remainder of their burst time
*/

#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include "queue.c"
#include "prime.c"

// CONSTANTS:
#define M_SIZE 100

// GLOBAL VARIABLES:
pid_t currentChild;
pid_t currentProcess;
bool timerExpired = false;
long unsigned int *lastPrime;
time_t schedulerTime = 0;
time_t lastTimeSlice = 0;

/*
    Function Name: SIGSTPHandler

    Input to Method:
        int sigNum - Signal number which triggered this handler

    Output (Return value):
        N/A

    Brief description of the task:
        Handle the SIGSTP signal, which is sent when the process is about to be suspended.
*/
void SIGTSTPHandler(int sig)
{
    // Print about to be suspended
    printf("Process %d: my PID is %d: I am about to be suspended...Highest prime number I found is %lu.\n", currentProcess, getpid(), *lastPrime);

    // Return proper signal
    signal(SIGTSTP, SIG_DFL);
}

/*
    Function Name: SIGTERMHandler

    Input to Method:
        int sigNum - Signal number which triggered this handler

    Output (Return value):
        N/A

    Brief description of the task:
        Handle the SIGTERM signal, which is sent when the process is about to be terminated.
*/
void SIGTERMHandler(int sig)
{
    // Print final message
    printf("\nProcess %d: my PID is %d: I am leaving the system. The largest prime I found was %lu \n", currentProcess, getpid(), *lastPrime);

    // Free last prime if it hasn't been freed already
    if(lastPrime != NULL)
    {
        free(lastPrime);
        lastPrime = NULL;
    }

    // Exit
    exit(0);
}

/*
    Function Name: SIGALRMHandler

    Input to Method:
        int sigNum - Signal number which triggered this handler

    Output (Return value):
        N/A

    Brief description of the task:
        Handle the SIGALRM signal, which is sent when the timer expires.
*/
void SIGALRMHandler(int sigNum)
{
    /* Handle SIGALRM here. */
    timerExpired = true;
    schedulerTime += lastTimeSlice;
}

/*
    Function Name: sendSignalToChildProc

    Input to Method:
        pid_t childPID - the process ID of the child process to send a signal to
        int signalNum - the signal constant to send to the child process using kill()

    Output (Return value):
        Either 0 for success of kill signal, or -1 for failure of sending signal

    Brief description of the task:
        Sends a SIGNAL NUMBER to a child process and returns if it was successful or not.
*/
int sendSignalToChildProc(pid_t childPID, int signalNum)
{
    // Kill Status will either = 0 or -1; 0 -> SUCCESS, -1 -> ERROR/FAILURE
    int killStatus = kill(childPID, signalNum); // SIGTSTP = SUSPEND, SIGCONT = RESUME
    return killStatus;
}

/*
    Function Name: beginRuntimeOfChild

    Input to Method:
        time_t timeSlice - The amount of time (in seconds) for the timer to run at

    Output (Return value):
        N/A

    Brief description of the task:
        Register a SIGALRM handler, and then start a timer for the amount of time specified by the timeSlice parameter.
*/
void beginRuntimeOfChild(time_t timeSlice)
{
    struct itimerval timer;
    lastTimeSlice = timeSlice;

    /* Install timer_handler as the signal handler for SIGALRM. */
    // signal(SIGALRM, timer_handler);  // NOTE: signal() is poorly defined, use sigaction
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &SIGALRMHandler;
    sigaction(SIGALRM, &sa, NULL);

    // it_value = "current value"
    // it_interval = "next value"
    // Timers decrement from it_value to zero, generate a signal, and reset it to it_interval.

    /* The timer goes off 1.0 second after installation of the timer. */
    timer.it_value.tv_sec = timeSlice; // _sec = seconds.
    timer.it_value.tv_usec = 0;        // _usec = microseconds.
    /* ... and every 1.0 second after that. */
    timer.it_interval.tv_sec = timeSlice;
    timer.it_interval.tv_usec = 0;

    /* Start a real timer. It counts down whenever this process is executing. */
    setitimer(ITIMER_REAL, &timer, NULL);
    timerExpired = false;
    while (!timerExpired)
    {
        // Wait for timer to expire
    }
}

// Main method
int main(int argc, char *argv[])
{
    // Fetch the time slice from the arguments
    time_t timeSlice = atoi(argv[2]);

    // Create our Q1
    struct Queue *q1 = initQueueStruct(q1, M_SIZE);
    newQueue(q1, M_SIZE);

    // Open file from first argument
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    fp = fopen(argv[1], "r");

    // Skip first line (header line)
    getline(&line, &len, fp);

    // Iterate over each line of file and add to queue
    while ((getline(&line, &len, fp)) != -1)
    {
        // Malloc space for PID and Burst Time
        pid_t *filePID = malloc(sizeof(pid_t) * len);
        time_t *fileBurstTime = malloc(sizeof(time_t) * len);

        // Scan the line for the PID and Burst Time
        sscanf(line, "%d%zu\n", filePID, fileBurstTime);

        // Create a new datum structure and initilize it's values
        struct Datum *newDatum = initDatumStruct(newDatum, *filePID, *fileBurstTime);
        newDatum->inputPID = *filePID;
        newDatum->inputBurst = *fileBurstTime;

        // Push the new datum to the queue
        enqueue_Push(q1, *newDatum);
    }

    // Initialize Q2
    struct Queue *q2 = initQueueStruct(q2, M_SIZE);
    newQueue(q2, M_SIZE);

    // Track if this is the first run
    bool first = true;

    // Last Process # and what we did with it
    char* nextHeader = malloc(sizeof(char) * 128);

    // Iterate over Q1 and fork each process
    while (!isEmpty(q1))
    {
        // Pop from queue to get first element to run
        Datum line1 = dequeue_Pop(q1);

        // Globally store current process
        currentProcess = line1.inputPID;

        // Create a child process
        pid_t childPid = fork();

        // We are in parent PID:
        if (childPid != 0)
        {
            // Update the PID of the child process
            line1.childPID = childPid;

            // Update global
            currentChild = childPid;

            // Calculate the time slice
            // long unsigned int slice = timeSlice;
            time_t slice = timeSlice;

            // If the burst time is less than the time slice, set the slice to the burst time
            if (line1.inputBurst <= timeSlice)
            {
                slice = line1.inputBurst;
            }

            // Print the first time
            if (first)
            {
                printf("Scheduler: Scheduling to Process %d (PID %d) for the time slice of %lu seconds.\n", line1.inputPID, childPid, slice);
            }
            else
            {
                // Second part of Suspending/Terminating message (must exit to see next Pid)
                printf("Scheduler: %s scheduling Process %d (Pid %d) for the time slice of %lu seconds.\n", nextHeader, line1.inputPID, childPid, slice);
            }

            // Start timer
            beginRuntimeOfChild(slice);

            printf("\nScheduler: Time Now: %lu seconds\n", schedulerTime);

            /* If we get here, timer has exited! */

            // Determine if we want to kill, or if we want to suspend process. If burst time is less than time slice, kill.
            if (line1.inputBurst <= timeSlice)
            {
                // Print message and flush output
                sprintf(nextHeader, "Terminating Process %d and", line1.inputPID);

                // Kill the process
                sendSignalToChildProc(childPid, SIGTERM); // SIGTERM is more graceful than SIGKILL
            }
            else
            {
                // Print message and flush output
                sprintf(nextHeader, "Suspending Process %d and moving it to FCFS queue and", line1.inputPID);

                // Suspend child process (SIGSTP)
                sendSignalToChildProc(childPid, SIGTSTP);

                // Push process to Q2
                enqueue_Push(q2, line1);
            }
        }
        // We are in the child PID:
        else
        {
            // Register SIGTSTP handler
            signal(SIGTSTP, SIGTSTPHandler);

            // Register SIGTERM handler
            signal(SIGTERM, SIGTERMHandler);

            // Start processing Prime Numbers
            lastPrime = malloc(sizeof(long unsigned int));
            *lastPrime = generateRandom10DigitNumber();

            // Print random number
            printf("\nProcess %d: my PID is %d: I just got started. I am starting with the number %lu to find the next prime number.\n", line1.inputPID, getpid(), *lastPrime);

            // Infinately search for primes
            while (true)
            {
                *lastPrime = findNextPrime(*lastPrime);
            }

            // Free last prime if it hasn't been freed already
            if(lastPrime != NULL)
            {
                free(lastPrime);
                lastPrime = NULL;
            }

            // Exit child process so we don't accidentally start looping again
            return 0;
        }

        // No longer the first time through
        first = false;
    }

    // Free last header, as we're done with it
    free(nextHeader);

    // Print message that we're switching to Q2
    printf("NO MORE PROCESSES IN Q1, MOVING TO QUEUE 2\n");

    // Reset first tracker
    first = true;

    // Iterate over Queue 2 and resume each process:
    while (!isEmpty(q2))
    {
        // Fetch first Q2 element to process
        Datum line1 = dequeue_Pop(q2);

        // If this is the first, we have to print a *slightly* different message
        if (first)
        {
            printf("Resuming process %d\n\n", line1.inputPID);
        }

        // Send signal to child process to resume:
        sendSignalToChildProc(line1.childPID, SIGCONT); // Assume it resumes the infinite while prime loop

        // Call upon parent signal handler to time this continued process; the timer:
        beginRuntimeOfChild((line1.inputBurst - timeSlice));

        // Now, process' burst has been completed, proceed to terminate:

        // Check to see if we aren't popping next go around; change standard output if so:
        if (isEmpty(q2))
        {
            printf("Scheduler: Time now is %lu seconds. Terminating process %d \n", schedulerTime, line1.inputPID);
        }
        else // If the queue isn't empty, we need to peek at the next process to run so we know what to print
        {
            Datum nextLine = peek(q2);
            printf("Scheduler: Time now is %lu seconds. Terminating Process %d and resuming Process %d \n", schedulerTime, line1.inputPID, nextLine.inputPID);
        }

        // Terminate the child (process)
        sendSignalToChildProc(line1.childPID, SIGTERM);

        // No longer the first time through
        first = false;
    }

    printf("\nScheduler: No more processes to run. Bye");

    // Cleanup
    // freeThisQueue(q2);
    // free(q2);
    // freeThisQueue(q1);
    // free(q1);
    return 0;
}