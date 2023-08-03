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
#include "queue.c"
#include "prime.c"

// CONSTANTS:
#define M_SIZE 100
pid_t currentChild;
bool timerExpired = false;

// Queues should have pid_t elements since each child process is uniquely identified by its PID

void timer_handler(int sigNum)
{
    /* Handle SIGALRM here. */
    printf("Current guy I stopped on: %u\n", currentChild);
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
    timer.it_value.tv_sec = timeSlice;  // _sec = seconds.
    timer.it_value.tv_usec = 0; // _usec = microseconds.
    /* ... and every 1.0 second after that. */
    timer.it_interval.tv_sec = timeSlice;
    timer.it_interval.tv_usec = 0;

    /* Start a real timer. It counts down whenever this process is executing. */
    setitimer(ITIMER_REAL, &timer, NULL);
    timerExpired = false;
    while (!timerExpired)
    {
    }
}

// Main method
int main(int argc, char *argv[])
{
    // printf("Test");

    time_t timeSlice = atoi(argv[2]);
    // Create a queue
    struct Queue *q1 = initQueueStruct(q1, M_SIZE);
    newQueue(q1, M_SIZE);

    // Open file from first argument
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    fp = fopen(argv[1], "r");

    // Skip first line (header line)
    getline(&line, &len, fp);

    // Iterate over each line of file and add to queue
    while ((read = getline(&line, &len, fp)) != -1)
    {
        // printf("Retrieved line of length %zu:\n", read);
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

    // struct Queue* q2 = initQueueStruct(q2, M_SIZE);
    // newQueue(q2, M_SIZE);
    // size_t q2Size = getQueueSize(q2);
    // Datum line1 = dequeue_Pop(q1);
    // pid_t line1PID = line1.inputPID;
    // unsigned long line1Burst = line1.inputBurst;
    // printf("The thing I popped has a file PID of %u, with a burst of %zu\n", line1PID, line1Burst);

    // // Exit
    // freeThisQueue(q2);
    // free(q2);
    // freeThisQueue(q1);
    // free(q1);

    while (!isEmpty(q1))
    {
        // Pop from queue to get first element to run
        Datum line1 = dequeue_Pop(q1);

        printf("Is Empty: %d\n", isEmpty(q1));

        // Create a child process
        pid_t childPid = fork();

        if (childPid != 0)
        {
            //beginRuntimeOfChild(timeSlice);
            //wait(NULL);
            // Do nothing
            printf("Parent of child %d\n", childPid);

            // Update the PID of the child process
            line1.childPID = childPid;
            currentChild = childPid;
            beginRuntimeOfChild(timeSlice, childPid);
            wait(NULL);
            //beginRuntimeOfChild(timeSlice, childPid);

        }
        else
        {
            printf("I AM CHILD AND I AM %d \n", childPid);

            // Start processing Prime Numbers
            long unsigned int prime = processPrime();

            printf("YO i found a prime %lu\n", prime);

            return 0;
        }
    }

    return 0;
}