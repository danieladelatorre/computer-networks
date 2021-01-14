/*
Name: Daniela Delatorre
Date: October 12, 2020
Title: Lab3-Programming UDP/IP Socket
Description: This part of the lab copies binary and text files from the source.txt file and writes it to the destination.txt file
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>	
#include <fcntl.h>

//When running the program make sure to write the name of the source file then the name of the destination file
int main(int argc, char *argv[]){
	//creates file pointers: one for source file and one for destination file
	FILE *fp;
	FILE *fp2;
	//creates a buffer
	char buffer[1];
	//first file pointer opens the source file
	fp = fopen(argv[1], "rb");
	//makes sure it is able to open the file
	if(fp==NULL){
		printf("Error opening the file for reading\n");
		exit(1);
	}
	//second file pointer opens the destination file
	fp2 = fopen(argv[2], "wb");
	//checks to make sure we can open the destination file
	if(fp2==NULL){
		printf("Error opening the file for writing\n");
		exit(1);
	}
	//while fread is not 0(there is still stuff to read from source file) then write that info to the destination file
	while(fread(&buffer, 1, 1, fp)!=0){
		fwrite(buffer, 1, 1, fp2);
	}
	printf("Successfully wrote the items from the source file to the destination file!\n");
	//closes the files
	fclose(fp);
	fclose(fp2);
	return 0;
}
	
