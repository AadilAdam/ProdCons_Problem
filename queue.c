/************************************************************************************************************************
NAME:         AADIL AHMED ADAM
CLASS:        CS590 SECTION 01
SEMESTER:     SUMMER 2016
DESCRIPTION:  FINAL EXAM PROJECT TO DEMOSTRATE THE USE OF PTHREAD LIBRARY TO WRITE A C PROGRAM THAT CREATES A NUMBER OF PRODUCER AND CONSUMER THRAEDS. THE PRODUCER THREADS WILL GENERATE RANDOM NUMBERS TO BE STORED IN A CIRCULAR QUEUE. THE CONSUMER THREADS WILL REMOVE THE ELEMENTS FROM THE CIRCULAR QUEUE.

*******************8*****************************************************************************************************/

//HEADER FILES
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#define size 100  // QUEUE SIZE


pthread_mutex_t pcmutex;
pthread_cond_t condcon, condpro;
int queue[size], front= -1, rear=-1;
pthread_t tid1,tid2;

int count= 0;
unsigned int seed2;

void *producer(void *pro_pr);
void *consumer (void *con_pr);

// producer function
void *producer(void *pro_pr)
{
	int rnum;	
	seed2=10;
        
	while(1)
	{
		tid1= pthread_self();

		//sleep for random time from 1 to 3
		int sleep_num= (rand() % 3)+1 ;
		sleep(sleep_num);
		
		//generate random number from 1 to 1000
		seed2++;
		rnum=(rand_r(&seed2)%1001)+1;

		// lock the mutex
		pthread_mutex_lock(&pcmutex);
		
		//check if queue is full
		while(queue_full()){
			printf("Queue Overflow... Wait for the consumer to consume...\n");
			pthread_cond_wait(&condpro, &pcmutex);   //if queue is full then tell the producer to wait for the consumer to consume
		}

		if (front==-1){
			front=0;
		}
		rear=(rear+1)% size;
		queue[rear]=rnum;           // add the random number generated to the queue from the rear side
                count = count + 1;        // increment the count to check if the numbers added to the queue.
 
		printf("Producer thread with TID=%lu added %d to slot %d\n", (unsigned long)tid1, rnum , rear);
			
		// unlock the mutex
		pthread_mutex_unlock(&pcmutex);
		
		//signal the consumer that it can consume.
		pthread_cond_signal(&condcon);
	}
	pthread_exit(0);
}


// consumer function.
void *consumer (void *con_pr)
{

	unsigned int seed;
	seed=10;
	srand(seed);
	
	while(1)
	{
		tid2=pthread_self();

		//sleep for random time from1 to 3
		int sleep_num= rand() % 4 ;
		sleep(sleep_num);

		// lock the mutex
		pthread_mutex_lock(&pcmutex);
	
		while(queue_empty()){
			printf("Queue Empty... Lets wait for the producer to add an item..\n");
			pthread_cond_wait(&condcon, &pcmutex);   // consumer needs to wait for the producer to add an item..
		}

		int rnum;
		rnum= queue[front];    //remove the item from the front 
		count = count -1 ;     // decrement the count value
		printf(" Consumer thread with TID= %lu removed %d from slot %d\n", (unsigned long)tid2, rnum, front);

		front=(front+1)% size;    // check if the front value reached the size of the queue or not.

		//unlock the mutex
		pthread_mutex_unlock(&pcmutex);

		//signal the producer to add items to the queue.
		pthread_cond_signal(&condpro);
	}
	pthread_exit(0);
}


int queue_empty()
{
	if(count == 0)
	{         
           //front=rear;
		return 1;
	}	
	return 0;
}
			
int queue_full()
{
	if(count == 100){ 
		return 1;
	}
	return 0;
}



// main function
int main()
{

pthread_t prod,cons;

pthread_mutex_init(&pcmutex, NULL);    // create the mutex lock

//initialize the condition variables for the producer and the consumer.
pthread_cond_init(&condpro, NULL);
pthread_cond_init(&condcon, NULL);

int num_pro, num_con;

// ask for the user input
printf(" Enter the number of Producers: ");
scanf("%d", &num_pro);

printf(" Enter the number of Consumers :");
scanf("%d", &num_con);
int i,j,err;
	while(1)
	{
	for(i=1;i<=num_pro;i++){
	//create producer thread
	err=pthread_create(&prod, 0, producer, NULL);
	if(err!=0)
		perror("Unable to create producer thread\n");
	}

	for(j=1;j<=num_con;j++){
	// create consumer thread
	err= pthread_create(&cons, 0, consumer, NULL);
	if(err!=0)
		perror("Unable to create consumer threads\n");
	}

	//Wait for the threads to finish
  	//Otherwise main might run to the end and kill the entire process when it exits
	pthread_join(prod, NULL);
	pthread_join(cons, NULL);

	// Cleanup would happen automatically at end of program
	pthread_cond_destroy(&condpro);
	pthread_cond_destroy(&condcon);
	
	//destroy the mutex
	pthread_mutex_destroy(&pcmutex);
	}
}
