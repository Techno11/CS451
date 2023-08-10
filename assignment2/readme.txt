To compile this program, mlfScheduler, do the following in terminal:
	gcc mlfScheduler.c
	
To run this program, mlfScheduler, do the following in terminal:
	./a.out input.txt [timeSlice#]
	
	***NOTE: The [timeSlice#] is to be replaced with an actual number.
	That is, to run with queue 1 time slice of 10, you would do the following:
		./a.out input.txt 10
		
LIMITATIONS:
	The scheduler, queues, prime algorithm, signal handling, and fork operations all work as expected.
	However, there are race conditions present in the program competing for stdout. 
	These race conditions hence produce some weird or out-of-order print output. 
	The three things competing for standard output are:
		1. Scheduler is competing to print start/end process output.
		2. New child process is competing to say it started.
		3. Old child process is competing in printing its last message to output.
	The slightly out-of-order output from these race conditions does NOT impact
	program output accuracy. 
