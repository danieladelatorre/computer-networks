/*
Name: Daniela Delatorre
Date: October 12, 2020
Title: Lab3-Programming UDP/IP Socket
Description: This part of the lab takes the file message from the client in step4c.c and writes it to the destination file
*/

// UDP Server
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(){
 //Declare socket file descriptor. All  Unix  I/O  streams  are  referenced  by  descriptors
 int sockfd;
 
 //Declare receiving buffer of size 1k bytes
 char name[] = "part4dest.txt";

 //Declare server address to which to bind for receiving messages and client address to fill in sending address
 struct sockaddr_in servAddr, clienAddr;
 socklen_t addrLen = sizeof(struct sockaddr);

 //Open a UDP socket, if successful, returns  a descriptor  associated  with  an endpoint 
 if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("Failure to setup an endpoint socket");
    exit(1);
 }
 
 //Setup the server address to bind using socket addressing structure
 servAddr.sin_family = AF_INET;
 servAddr.sin_port = htons(5000); //Port 5000 is assigned
 servAddr.sin_addr.s_addr = INADDR_ANY; //Local IP address of any interface is assigned (generally one interface IP address)

 //Set address/port of server endpoint for socket socket descriptor
 if ((bind(sockfd, (struct sockaddr *)&servAddr, sizeof(struct sockaddr))) < 0){
	perror("Failure to bind server address to the endpoint socket");
    exit(1);
 }
 
//creates int variable to hold file descriptor
int fp2;

//Sever continuously waits for messages from client, then writes incoming messages to the destination file
 while (1){
    char rbuf[1024];
    fp2 = open(name, O_WRONLY | O_APPEND);
    printf("Server waiting for messages from client: \n");
    int nr = recvfrom(sockfd, rbuf, 1024, 0, (struct sockaddr *)&clienAddr, &addrLen);
    rbuf[nr] = '\0';
    write(fp2, rbuf, nr+1);
    printf("%s\n", rbuf);
    printf("Successfully wrote items from source file to the step4dest.txt file\n");	
    printf("File closed\n");
    close(fp2);
 }
 return 0;
}
