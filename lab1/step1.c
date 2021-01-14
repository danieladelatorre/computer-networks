/*
Name: Daniela Delatorre
Date: 09/24/2020
Title: Lab1-Unix/Linux Commands and Overview of C Programming
Description: This part of the lab forks and creates a child process along with a parent process. The parent process prints "Parent Process" followed by the iteration and the child process prints "Child Process" also followed by the iteration number.
*/

#include <stdio.h>      /* printf, stderr */
#include <sys/types.h>  /* pid_t */
#include <unistd.h>     /* fork */
#include <stdlib.h>     /* atoi */
#include <errno.h>      /* errno */ 

int main(int argc, char *argv[]){
	pid_t  pid;
	int i, n = atoi(argv[1]);i	//n converts the second command line argument to an int that represents the delay
	printf("\n Before forking.\n");
	pid = fork();			//forks and creates both a parent and child process
	if (pid == -1) {		//if there was an error while forking
		fprintf(stderr, "can't fork, error %d\n", errno);
	}
	if (pid){
		// Parent process
		for (i=0;i<100;i++) {
			printf("\t \t \t Parent Process %d \n",i);
			usleep(n);
		}
	}
	else{
		// Child process
		for (i=0;i<100;i++) {
			printf("Child process %d\n",i);
			usleep(n);
		}
	}
	return 0;
}
