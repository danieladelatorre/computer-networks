#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

int main(){
	//Declare socket file descriptor. All Unix I/O streams are referenced by descriptor. nr is the number of received bytes
	int sockfd, nr;

	//Declare sending and receiving buffer of size 1k bytes
	char sbuf[1024], rbuf[1024];

	//Declare the server address to connect to
	struct sockaddr_in servAddr;
	struct hostent *host;

	//Converts domain names to IP address via DNS
	host = (struct hostent *)gethostbyname("localhost");

	//Open a socket, if successful, returns  a descriptor  associated  with  an endpoint 
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Failure to setup an endpoint socket");
		exit(1);
	}

	//Set the server address to send using socket addressing structure
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(5000);
	servAddr.sin_addr = *((struct in_addr *)host->h_addr);

	//Connect to the server
	if(connect(sockfd, 

	return 0;
}
