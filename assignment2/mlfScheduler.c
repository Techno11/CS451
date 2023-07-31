#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

/*
    Function Name: isPrime

    Input to Method: 
        num - The number to be prime checked

    Output (Return value): 
        Whether or not the number is prime

    Breif description of the task:
        Checks if the number is prime or not
*/
bool isPrime(long unsigned int num)
{
    if (num <= 1)
        return false;

    for (long unsigned int i = 2; i * i <= num; i++)
    {
        if (num % i == 0)
            return false;
    }

    return true;
}

/*
    Function Name: findNextPrime

    Input to Method: 
        startNum - The number to start checking from

    Output (Return value): 
        The next prime number

    Breif description of the task:
        Finds the next prime number after the given number
*/
long unsigned int findNextPrime(long unsigned int startNum)
{
    long unsigned int currentNum = startNum + 1;

    while (true)
    {
        if (isPrime(currentNum))
            return currentNum;

        currentNum++;
    }
}

/*
    Function Name: generateRandom10DigitNumber

    Input to Method: 
        N/A

    Output (Return value): 
        Random 10 digit number

    Breif description of the task:
        Generates a random 10 digit number
*/
long unsigned int generateRandom10DigitNumber()
{
    long unsigned int minNumber = 1000000000; // 10^9
    long unsigned int maxNumber = 9999999999; // 10^10 - 1

    // Seed the random number generator with the current time
    srand((unsigned int)time(NULL));

    // Generate a random number between min_number and max_number
    return (rand() % (maxNumber - minNumber + 1)) + minNumber;
}

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
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec =0;
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
    long unsigned int starting_number = generateRandom10DigitNumber();
    long unsigned int next_prime = findNextPrime(starting_number);

    printf("Starting number: %lu\n", starting_number);
    printf("Next prime number: %lu\n", next_prime);
    // Exit
    return 0;
}