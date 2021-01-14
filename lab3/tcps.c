#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

int main(){
	//Declare socket file descriptor. All  Unix  I/O  streams  are  referenced by descriptors
	int sockfd;

	//Declare buffers of size 1k bytes
	char rbuf[1024], sbuf[1024];

	//Declare server address to which to bind for receiving messages and client address to fill in sending address
	struct sockaddr_in servAddr, clienAddr;
	
	//Open the TCP socket
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("There was an error in creating the socket\n");
		exit(1);
	}

	//Set up the server address to bind using socket addressing strucutre
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(5000); //Port 5000 is assigned
	servAddr.sin_addr.s_addr = INADDR_ANY; //Local IP address of any interface is assigned (generally one interface IP address)
	
	//Set address/port of server endpoint for socket socket descriptor
	if((bind(sockfd, (struct sockaddr *)&servAddr, sizeof(struct sockaddr))) <0){
		printf("Failure to bind server address to the endpoint socket\n");
		exit(1);
	}

	printf("Server waiting for client at port 5000\n");

	//Server listening to socket endpoint and can queue 5 client requests
	listen(sockfd, 5);
	
	return 0;
}
