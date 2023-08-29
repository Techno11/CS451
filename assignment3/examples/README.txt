NOTE: To run the current code from terminal, using the 2nd example ($ are CMD tokens, do not type $ in the command):
-----------------------------------------------------------------------------------------------------
   $ gcc elevator.c -o elevator.o -lpthread
   $ ./elevator.o -p 5 -f 11 -w 10 < ./examples/deterministic_example_2.txt
-----------------------------------------------------------------------------------------------------

-----------------------------------------------------------------------------------------------------------------------
The Deterministic examples must be run with the following parameters:

    $ ./elevator.o -p 1 -f 11 < ./examples/deterministic_example_1.txt


    $ ./elevator.o -p 5 -f 11 –w 10 < ./examples/deterministic_example_2.txt


    $ ./elevator.o -p 5 -f 12 -w 10 < ./examples/deterministic_example_3.txt

WHERE -p = number of people, -f = number of floors, -w = maximum wandering time, and < is the file to read stdin from.
-----------------------------------------------------------------------------------------------------------------------

AS A SANITY CHECK... The following commands should work (standalone) IDENTICAL to above....
To run, First, copy and run the command portion. (Copy the command, paste, and press enter. Nothing should happen...)
Then, copy ALL the numbers, as well as the newline AFTER the numbers and paste it in the terminal. Press Enter.

// Identical to deterministic_example_1.txt
./elevator.o -p 1 -f 11
3 2 5 10 5 0 5

// Identical to deterministic_example_2.txt
./elevator.o -p 5 -f 11 -w 10
3 1 1 10 1 0 5
3 3 1 10 2 0 5
3 5 1 10 3 0 5
3 7 1 10 4 0 5
3 9 1 10 5 0 5

// Identical to deterministic_example_3.txt
./elevator.o -p 5 -f 12 -w 10
10 2 1 11 1 1 5 2 1 3 1 4 1 5 1 6 1 7 1 0 2
10 4 1 11 2 1 5 3 1 4 1 5 1 6 1 7 1 8 1 0 2
10 6 1 11 3 1 5 4 1 5 1 6 1 7 1 8 1 9 1 0 2
10 8 1 11 4 1 5 5 1 6 1 7 1 8 1 9 1 10 1 0 2
10 10 1 11 5 1 5 6 1 7 1 8 1 9 1 10 1 11 1 0 3
