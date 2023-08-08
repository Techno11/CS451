/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 2
        Date of Submission: 8/9/2023
        Name of this file: prime.h
        Short Description of contents:
            This file contains the function prototypes for the functions in prime.c
*/

#ifndef PRIME_H
#define PRIME_H

#include <stdbool.h>

/*======== SEE 'prime.c' FOR FUNCTION HEADER COMMENTS ========*/
bool isPrime(long unsigned int num);
long unsigned int findNextPrime(long unsigned int startNum);
long unsigned int generateRandom10DigitNumber();

#endif // PRIME_H