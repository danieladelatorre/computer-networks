/*
Name: Daniela Delatorre
Date: October 5 2020
Title: Lab2 - Multithreading in C and network commands
Description: Uses N threads to perform matrix multiplication on matrices A and B and store the result in matrix C
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define M 1024
#define N 1024
#define L 1024

double matrixA[N][M], matrixB[M][L], matrixC[N][L];

pthread_t matrix_thread[N];


void *thread_function(void *arg){
	int a, b;
	int argument = (int)(size_t *)(arg);
	int temp; 
	for(a=0; a<L; a++){
		temp = 0;
		for(b=0; b<M; b++){
			temp+=((matrixA[argument][b])*(matrixB[b][a]));
		}
		matrixC[argument][a] = temp;
	}	
	return 0;
}


int main(){
	srand(time(NULL));
	int i, j;

	//initializes matrixA
	for(i=0; i<N; i++){
		for(j=0; j<M; j++){
			matrixA[i][j] = rand()%10;
			printf("%f ", matrixA[i][j]);
		}
		printf("\n");
	}

	//initializes matrixB
	for(i=0; i<M; i++){
		for(j=0; j<L; j++){
			matrixB[i][j] = rand()%10;
			printf("\t\t\t %f ", matrixB[i][j]);
		}
		printf("\n");
	}

	for(i=0; i<N; i++){
		pthread_create(&matrix_thread[i], NULL, thread_function, (void *) (size_t)i); 
	}
	for(i=0; i<N; i++){
		pthread_join(matrix_thread[i], NULL);
	}

	for(i=0; i<N; i++){
		for(j=0; j<L; j++){
			printf("%f ", matrixC[i][j]);
		}
		printf("\n");
	}

	return 0;
}
