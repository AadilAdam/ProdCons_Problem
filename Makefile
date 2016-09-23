//save the file as queue.c and then create a MAKEFILE which would conatin the below code

queue: queue.c
	gcc -o queue queue.c -pthread
clean: 
	rm -f queue

//save this file and make use of the command "$make "to execute the program.
This will create an object file which will be stored as "queue". Then run the program by "$./queue"
