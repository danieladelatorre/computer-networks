/*
Name: Daniela Delatorre
Date: 09/24/2020
Title: Lab1-Unix/Linux Commands and Overview of C Programming
Description: This part of the lab does the same as Step1 except that instead of creating processes using fork, it creates lightweight processes or threads. The first thread prints out "Parent Thread" followed by the iteration number and the second thread prints "Child Thread" also followed by the iteration number.
*/

#include <stdio.h>      /* printf, stderr */
#include <pthread.h>     /* pthread */
#include <stdlib.h>     /* atoi */
#include <errno.h>      /* errno */ 

#define NUMTHREADS 2		//we want two seperate threads
pthread_t threads[NUMTHREADS];	//array of threads

//creates a struct so I can pass multiple arguments to the thread function
struct thread_args{		
	int thread_number;
	int delay;
};

//this thread_function is a function executed by the threads
void *thread_function(void *arg){
	struct thread_args *args = (struct thread_args *)arg;	//converts the void *args arguement to a struct pointer
	int i;
	int value = args->thread_number;			//value is the thread number from the args structure
	int n = args->delay;					//n is the delay from the args structure
	if(value==0){						//if the thread is the first thread, then it executes the parent section
		for(i=0; i<100; i++){
			printf("Parent Thread %d\n", i);
			usleep(n);
		}
	}
	else{							//if the thread is the second thread, then it executes the child section
		for(i=0; i<100; i++){
			printf("\t \t \t Child Thread %d\n", i);
			usleep(n);
		}
	}
	return 0;
}

int main(int argc, char *argv[]){

	int i, n = atoi(argv[1]);
	printf("\n Before using threads.\n");
	
	struct thread_args *args = malloc(sizeof(struct thread_args));	//allocates memory for the first thread's arguments
	struct thread_args *args2 = malloc(sizeof(struct thread_args));	//allocates memory for the second thread's arguments

	for(i=0; i<NUMTHREADS; i++){					//loop needed to create the threads
		if(i==0){
			args->thread_number = i;			//initiates the thread_number to 0 to show it's the first thread created
			args->delay = n;				//initiates n to be the delay
			pthread_create(&threads[i], NULL, thread_function, args);	//creates the first thread and has it execute the thread_function 
		}
		else{
			args2->thread_number = i;			//initiates the thread_number to show it's the second thread created
			args2->delay = n;				//initiates n to be the delay
			pthread_create(&threads[i], NULL, thread_function, args2);	//creates the second thread and has it execute the thread_function
		}
	}
	for(i=0; i<NUMTHREADS; i++){
		pthread_join(threads[i], NULL);				//joins the threads
	}
	
	return 0;
}
