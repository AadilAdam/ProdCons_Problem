queue: queue.c
	gcc -o queue queue.c -pthread
clean: 
	rm -f queue
