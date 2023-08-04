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
pid_t currentChild;
bool timerExpired = false;
int fd[2]; // pipe for child processes to use
long unsigned int *lastPrime;
time_t schedulerTime = 0;

void timer_handler(int sigNum)
{
    /* Handle SIGALRM here. */
    timerExpired = true;
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

void beginRuntimeOfChild(time_t timeSlice, pid_t childPid)
{
    struct itimerval timer;

    /* Install timer_handler as the signal handler for SIGALRM. */
    // signal(SIGALRM, timer_handler);  // NOTE: signal() is poorly defined, use sigaction
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &timer_handler;
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
        // Read from pipe while timering
        read(fd[0], lastPrime, sizeof(lastPrime));
    }
}

// Main method
int main(int argc, char *argv[])
{
    time_t timeSlice = atoi(argv[2]);
    // Create a queue
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
        // void enqueue_Push(Queue* queue, Datum newProc)
        pid_t *filePID = malloc(sizeof(pid_t) * len);
        unsigned long *fileBurstTime = malloc(sizeof(unsigned long) * len);
        sscanf(line, "%d%zu\n", filePID, fileBurstTime);
        struct Datum *newDatum = initDatumStruct(newDatum, *filePID, *fileBurstTime);
        newDatum->inputPID = *filePID;
        newDatum->inputBurst = *fileBurstTime;
        enqueue_Push(q1, *newDatum);
        printf("PID: %d, Burst: %zu\n", *filePID, *fileBurstTime);
    }

    struct Queue *q2 = initQueueStruct(q2, M_SIZE);
    newQueue(q2, M_SIZE);

    // Open a pipe for all child processes to use
    pipe(fd);

    // Initilize lastPrime
    lastPrime = malloc(sizeof(lastPrime));

    // Initial print
    bool first = true;

    // Iterate over Q1 and fork each process
    while (!isEmpty(q1))
    {
        // Pop from queue to get first element to run
        Datum line1 = dequeue_Pop(q1);

        // Create a child process
        pid_t childPid = fork();

        if (childPid != 0)
        {
            // Update the PID of the child process
            line1.childPID = childPid;

            // Update global
            currentChild = childPid;

            // Calculate the time slice
            long unsigned int slice = timeSlice;

            // If the burst time is less than the time slice, set the slice to the burst time
            if (line1.inputBurst <= timeSlice)
            {
                slice = line1.inputBurst;
            }

            // Print the first time
            if (first)
            {
                printf("Scheduling to Process %d (PID %d) for the time slice of %lu seconds.\n", line1.inputPID, childPid, slice);
            }
            else
            {
                printf("scheduling Process %d (Pid %d) for the time slice of %lu seconds.\n", line1.inputPID, childPid, slice);
            }

            schedulerTime += slice;

            // Start timer
            beginRuntimeOfChild(slice, childPid);

            printf("Scheduler: Time Now: %lu seconds\n", schedulerTime);

            /* If we get here, timer has exited */

            // Determine if we want to kill, or if we want to suspend process. If burst time is less than time slice, kill.
            if (line1.inputBurst <= timeSlice)
            {
                // Print message and flush output
                printf("Terminating Process %d and scheduling ", line1.inputPID);
                fflush(stdout);

                // Kill the process
                sendSignalToChildProc(childPid, SIGKILL);
            }
            else
            {
                // Print message and flush output
                printf("Suspending Process %d and moving it to FCFS queue and ", line1.inputPID);
                fflush(stdout);

                // Suspend child process (SIGSTP)
                sendSignalToChildProc(childPid, SIGSTOP);

                // Push process to Q2
                enqueue_Push(q2, line1);
            }
        }
        else
        {
            // Close read end of pipe, not required
            close(fd[0]);

            // Start processing Prime Numbers
            long unsigned int num = generateRandom10DigitNumber();

            // Print random number
            printf("Process %d: my PID is %d: I just got started. I am starting with the number %lu to find the next prime number.\n", line1.inputPID, getpid(), num);

            // Infinately search for primes
            while (true)
            {
                num = findNextPrime(num);
                int n = write(fd[1], &num, sizeof(num));
            }

            return 0;
        }
        first = false;
    }

    printf("NO MORE PROCESSES IN Q1, MOVING TO QUEUE 2\n");

    // Cleanup
    // freeThisQueue(q2);
    // free(q2);
    // freeThisQueue(q1);
    // free(q1);
    // free(lastPrime);
    return 0;
}