#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include "queue.c"

// CONSTANTS:
#define M_SIZE 100

//Queues should have pid_t elements since each child process is uniquely identified by its PID

void timer_handler(int sigNum)
{
    /* Handle SIGALRM here. */
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

void beginRuntimeOfChild()
{
    struct itimerval timer;

    /* Install timer_handler as the signal handler for SIGALRM. */
    //signal(SIGALRM, timer_handler);  // NOTE: signal() is poorly defined, use sigaction
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &timer_handler;
    sigaction(SIGALRM, &sa, NULL);

    // it_value = "current value"
    // it_interval = "next value"
    // Timers decrement from it_value to zero, generate a signal, and reset it to it_interval.

    /* The timer goes off 1.0 second after installation of the timer. */
    timer.it_value.tv_sec = 1; // _sec = seconds.
    timer.it_value.tv_usec =0; // _usec = microseconds.
    /* ... and every 1.0 second after that. */
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec =0;

    /* Start a real timer. It counts down whenever this process is executing. */
    setitimer (ITIMER_REAL, &timer, NULL);
    while (1) { }
}

// Main method
int main(void)
{
    printf("Test");

    Queue q2;
    newQueue(&q2, M_SIZE);
    enqueue_Push(&q2, 1234); /*TODO: BUG - pid_t* array is NOT making an array, instead, a negative number.*/
    enqueue_Push(&q2, 5678);
    pid_t poppedItem = dequeue_Pop(&q2);
    printf("I am a queue of size %zu, and I popped %d", getQueueSize(&q2), poppedItem);
    // Exit
    freeThisQueue(&q2);
    return 0;
}