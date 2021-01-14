/*
Name: Daniela Delatorre
Date: October 12, 2020
Title: Lab3-Programming UDP/IP Socket
Description: This part of the lab copies binary and text files from the source.txt file and writes it to the destination2.txt file using system calls
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>	
#include <fcntl.h>
#include <unistd.h>

//when running the program type the source file before the destination file
int main(int argc, char *argv[]){
	//creates a character buffer to hold the data read from the source file
	char buffer[110];
	//Open() returns the file descriptors for the files in read only mode
	int fp = open(argv[1], O_RDONLY);
	//makes sure it was able to create a valid file descriptor
	if(fp<0){
		printf("Error trying to open the source file for reading\n");
		exit(1);
	}
	//creates a new file descriptor for the desination2.txt file, if not already created then it creates the file
	int fp2 = open(argv[2], O_WRONLY | O_CREAT);
	//makes sure it was able to create a valid file descriptor
	if(fp2<0){
		printf("Error trying to open the source file for reading\n");
		exit(1);
	}
	//size variable holds the number of items read from the source file
	int size = read(fp, buffer, 100);
	//while there are still items to read from the source file, write items to the destination file
	while(size!=0){
		write(fp2, buffer, size);
		size = read(fp, buffer, 100);
	}
	printf("Items have successfully been written from the source file to the destination file!\n");
	//closes the files 
	close(fp);
	close(fp2);
	return 0;
}
	
