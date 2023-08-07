/*
        Author: Brendan Sting, Soren Zaiser
        Assignment Number: 2
        Date of Submission: 7/24/2023
        Name of this file: prime.h
        Short Description of contents:
            This file contains the function prototypes for the functions in prime.c
*/

#ifndef PRIME_H
#define PRIME_H


bool isPrime(long unsigned int num);
long unsigned int findNextPrime(long unsigned int startNum);
long unsigned int generateRandom10DigitNumber();

#endif