/*
Name: Daniela Delatorre
Date: October 12, 2020
Title: Lab3-Programming UDP/IP Socket
Description: This part of the lab takes file input from the source file and sends it to the udp server in step4s.c
*/

//UDP Client
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
 //Declare socket file descriptor. All  Unix  I/O  streams  are  referenced  by  descriptors
 int sockfd; 

 //Declare sending buffer of size 1k bytes

 //declare the variable for the name of the file
 char name[20]; 

 //Declare server address
 struct sockaddr_in servAddr; 

 //Converts  domain  names   into  numerical  IP  addresses  via  DNS
 struct hostent *host;
 host = (struct hostent *)gethostbyname("localhost"); //Local host runs the server. You may use "127.0.0.1": loopback IP address
 
 //Open a socket, if successful, returns  a descriptor  associated  with  an endpoint 
 if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("Failure to setup an endpoint socket");
    exit(1);
 }

 //Set the server address to send using socket addressing structure
 servAddr.sin_family = AF_INET;
 servAddr.sin_port = htons(5000);
 servAddr.sin_addr = *((struct in_addr *)host->h_addr);

 //declares the file descriptor
 int fp;
 //Client to send messages to the server continuously using UDP socket 
 while(1){
  char sbuf[1024]; 
  printf("Client: Type the name of the source file to send to the server\n");
  scanf("%s", name);
  fp = open(name, O_RDONLY);
  int size = read(fp, sbuf, 1024);
  while(size!=0){
 	sendto(sockfd, sbuf, size, 0, (struct sockaddr *)&servAddr, sizeof(struct sockaddr));	
	size = read(fp, sbuf, 1024);
  }
  close(fp);
 }
 return 0;
}
