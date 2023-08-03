#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

/*
    Function Name: isPrime

    Input to Method:
        num - The number to be prime checked

    Output (Return value):
        Whether or not the number is prime

    Breif description of the task:
        Checks if the number is prime or not
*/
bool isPrime(long unsigned int* num)
{
    if (*num <= 1)
        return false;

    for (long unsigned int i = 2; i * i <= *num; i++)
    {
        if (*num % i == 0)
            return false;
    }

    return true;
}

/*
    Function Name: findNextPrime

    Input to Method:
        startNum - Pointer to the number to start checking from

    Output (Return value):
        N/A

    Brief description of the task:
        Finds the next prime number after the given number
*/
void findNextPrime(long unsigned int* startNum)
{
    // Increase the number by 1
    *startNum++;

    while (true)
    {
        if (isPrime(startNum))
            return;

        *startNum++;
    }
}

/*
    Function Name: generateRandom10DigitNumber

    Input to Method:
        Pointer to memory location to store the random number

    Output (Return value):
        N/A

    Brief description of the task:
        Generates a random 10 digit number
*/
long unsigned int generateRandom10DigitNumber(long unsigned int* ptr)
{
    // Set the seed
    srand(time(NULL));

    long unsigned int minNumber = 1000000000; // 10^9
    long unsigned int maxNumber = 9999999999; // 10^10 - 1

    // Generate a random number between min_number and max_number
    ptr = (rand() % (maxNumber - minNumber + 1)) + minNumber;
}