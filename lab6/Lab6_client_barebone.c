/*
Name: Daniela Delatorre
Date: November 2, 2020
Title: Lab6-Stop and Wait for an Unreliable Channel, with Loss
Description: This lab is very similar to lab 5 except that it includes a timer, so that whenever the ACK gets lost from the server side, after 10 seconds, it will resend the packet instead of waiting forever to receive the ACK
*/

#include <arpa/inet.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>	
#include <netdb.h>
#include <unistd.h>
#include "lab6.h"

#define PLOSTMSG 11

int getChecksum(Packet packet){
	packet.header.cksum = 0;
	int checksum = 0; 
	char *ptr = (char *)&packet;
	char *end = ptr + sizeof(Header) + packet.header.len;

	while(ptr < end){
		checksum ^= *ptr++;
	}
	return checksum;
}

void logPacket(Packet packet){
	printf("Packet{ header: { seq_ack: %d, len: %d, cksum: %d }, data: \"",
		packet.header.seq_ack,
		packet.header.len,
		packet.header.cksum);
	fwrite(packet.data, (size_t)packet.header.len, 1, stdout);
	printf("\" }\n");
}

int number = 0;
void ClientSend(int sockfd, const struct sockaddr *address, socklen_t addrlen, Packet packet, unsigned retries) {
    while (1) {
    	printf("RECVED packetform the main ()\n");
	logPacket(packet);
	printf("The number is %d\n", number);
	int random = (rand() % 11);
	printf("Random number is  %d\n", random);
	
	if(random < 7){
		packet.header.seq_ack = number;
		printf("sending the right seq\n");
	} else{
		packet.header.seq_ack = 70;
		printf("sending the wrong seq\n");
	}

	packet.header.len = strlen(packet.data);

	if(random < 5){
		packet.header.cksum = getChecksum(packet);
            	printf("sending the right check\n");
	} else {
		printf("sending the wrong check\n");
	}
        // log what is being sent
        printf("Created: ");
        logPacket(packet);

        // Simulate lost packet.
        if (rand() % PLOSTMSG == 0) {
            printf("Dropping packet\n");
        } else {
            printf("Sending out packet\n");
	    logPacket(packet);
	    sendto(sockfd, &packet, sizeof(packet), 0, address, addrlen);
	    printf("Sent ACK: %d and CheckSum: %d\n", packet.header.seq_ack,packet.header.cksum);   
        }

        // wait until either a packet is received or timeout, i.e using the select statement
        struct timeval tv;
	int rv;

	fd_set readfds;
	fcntl(sockfd, F_SETFL, O_NONBLOCK);

	FD_ZERO(&readfds);
	FD_SET(sockfd, &readfds);

        tv.tv_sec = 1;
        tv.tv_usec = 0;
        //******STUDENT WORK******
    	rv = select (sockfd + 1, &readfds, NULL, NULL, &tv);
	if(rv == 0){
		printf("Timeout on trial %d. Now trying again\n", retries);
		retries++;
	} else {
        	// receive an ACK from the server
        	Packet recvpacket;
        	//******STUDENT WORK******
		recvfrom(sockfd, &recvpacket, sizeof(recvpacket), 0, address, &addrlen);
        	// log what was received
        	printf("Received ACK %d, checksum %d - ", recvpacket.header.seq_ack, recvpacket.header.cksum);

        	// validate the ACK
        	if (recvpacket.header.cksum != getChecksum(recvpacket)){
            	// bad checksum, resend packet
            		printf("Bad checksum, expected %d\n", getChecksum(recvpacket));
        	} else if (recvpacket.header.seq_ack != packet.header.seq_ack) {
            		// incorrect sequence number, resend packet
            		printf("Bad seqnum, expected %d\n", packet.header.seq_ack);
        	} else {
            		// good ACK, we're done
            		printf("Good ACK\n");
			number = !number;
            		break;
        	}
    	}
    }

    printf("\n");
}

int main(int argc, char *argv[]) {
    // check arguments
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <ip> <port> <infile>\n", argv[0]);
        return 1;
    }

    // seed the RNG
    srand((unsigned)time(NULL));

    // create and configure the socket
    int sockfd;
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
	perror("Error creating the socket\n");
	exit(1);
    }

    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    socklen_t length = sizeof(struct sockaddr);
    struct hostent *host;
    host = (struct hostent *)gethostbyname(argv[1]);
    address.sin_family = AF_INET;
    address.sin_port = htons(atoi(argv[2]));
    address.sin_addr = *((struct in_addr *)host->h_addr);

    // initialize the server address structure
    //******STUDENT WORK******
    // open file
    FILE *file = fopen(argv[3], "rb");
    if (file == NULL) {
        perror("fopen");
        return 1;
    }
    
    int byte;
    // send file contents to server
    Packet packet;
    while((byte = fread(packet.data, 1, sizeof(packet.data), file)) > 0) {
	ClientSend(sockfd, (struct sockaddr *)&address, length, packet, 0);
	memset(&packet, 0, sizeof(packet));
    }

    // send zero-length packet to server to end connection according the description provided in the Lab manual
    packet.header.seq_ack = !number;
    packet.header.len = 0;
    packet.header.cksum = getChecksum(packet);
    packet.data[0] = '\0';

    ClientSend(sockfd, (struct sockaddr *)&address, length, packet, 0);
    
    // clean up
    fclose(file);
    close(sockfd);
    return 0;
}
