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

    Brief description of the task:
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

    Brief description of the task:
        Generates a random 10 digit number
*/
long unsigned int generateRandom10DigitNumber()
{
    // Set the seed
    srand(time(NULL));

    long unsigned int minNumber = 1000000000; // 10^9
    long unsigned int maxNumber = 9999999999; // 10^10 - 1

    // Generate a random number between min_number and max_number
    return (rand() % (maxNumber - minNumber + 1)) + minNumber;
}