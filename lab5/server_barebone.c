/*
Name: Daniela Delatorre
Date: November 2, 2020
Title: Lab 5-Stop and Wait for an Unreliable Channel
Description: This part of the assignment is the server side of the UDP rdt2.2 protocol. The server has 4 states: Waiting for 0, Waiting for ACK 0, Waiting for 1, and Waiting for ACK 1. The sender starts by waiting for a call from the client and once it receives the call it sends packet 0 to the client. It then waits for an ACK from the client and if it receives the correct ACK it moves to the next state where it waits for call 1 from the client and so forth...
*/

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include "lab5.h"


// NOTE: beware of sign extension if the checksum is stored in a char, then converted to an int!
int getChecksum(Packet packet) {
    packet.header.cksum = 0;

    int checksum = 0;
    char *ptr = (char *)&packet;
    char *end = ptr + sizeof(Header) + packet.header.len;

//Please find an error from the remaining part getChecksum() function 
//******STUDENT WORK****** 
    while (ptr < end) {
        checksum ^= *ptr++;
    }

    return checksum;
}

void logPacket(Packet packet) {
    printf("Packet{ header: { seq_ack: %d, len: %d, cksum: %d }, data: \"",
            packet.header.seq_ack,
            packet.header.len,
            packet.header.cksum);
    fwrite(packet.data, (size_t)packet.header.len, 1, stdout);
    printf("\" }\n");
}

//Sending a ACK to the client, i.e., letting the client know what was the status of the packet it sent.
void ServerSend(int sockfd, const struct sockaddr *address, socklen_t addrlen, int seqnum) {
    Packet packet;
	//-Prepare the packet headers
	//-send the packet
    int random = (rand() % 11);
    printf("Random number is %d\n", random);
    if(random < 7){
	packet.header.seq_ack = seqnum;
	printf("Sending the right sequence number\n");
    } else {
	packet.header.seq_ack = 30;
	printf("Sending the wrong sequence number\n");
    }
    packet.header.len = 0;
    if(random < 5){
	packet.header.cksum = getChecksum(packet);
	printf("Sending the correct checksum\n");
    } else {
	printf("Sending the wrong checksum\n");
    }
    
    sendto(sockfd, &packet, sizeof(packet), 0, address, addrlen);

    printf("Sent ACK %d, checksum %d\n", packet.header.seq_ack, packet.header.cksum);
    return;
}

Packet ServerReceive(int sockfd, struct sockaddr *address, socklen_t *addrlen, int seqnum) {
    Packet packet;

    while (1) {
        //recv packets from the client
	recvfrom(sockfd, &packet, sizeof(packet), 0, address, addrlen);

        // log what was received
        printf("Received: ");
        logPacket(packet);

        if (packet.header.cksum != getChecksum(packet)) {
            printf("Bad checksum, expected checksum was: %d\n", getChecksum(packet));
            ServerSend(sockfd, address, *addrlen, !seqnum);
        } else if (packet.header.seq_ack != seqnum) {
            printf("Bad seqnum, expected sequence number was:%d\n", seqnum);
            ServerSend(sockfd, address, *addrlen, !seqnum);
        } else {
            printf("Good packet\n");
            ServerSend(sockfd, address, *addrlen, seqnum);
	    printf("writing to the file %s\n", packet.data);
            break;
        }
    }

    printf("\n");
    return packet;
}


int main(int argc, char *argv[]) {
    // check arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <port> <outfile>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // seed the RNG
    srand((unsigned)time(NULL));

    // create a socket
    int sockfd;
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
	perror("Error creating socket endpoint!\n");
	return 0;
    }

    // initialize the server address structure using argv[1]
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(atoi(argv[1]));
    address.sin_addr.s_addr = INADDR_ANY;

    // bind the socket
	// [Question]: Do we need to bind the socket on the client side? Why?/Why not?
    	//My Answer: No, we don't have to bind the socket on the client side because the sendto function call makes the bind for us in the client side
    if((bind(sockfd, (struct sockaddr *)&address, sizeof(struct sockaddr))) < 0){
	perror("Failed to bind the socket\n");
	return 0;
    }

    // open file using argv[2]
    FILE *file;
    file = fopen(argv[2], "wb");

    if(file==NULL){
	printf("Error opening the file for writing\n");
	exit(1);
    }
    // get file contents from client
    int seqnum = 0;
    Packet packet;
    struct sockaddr_in clientaddr;
    socklen_t clientaddr_len = sizeof(clientaddr);
    do {
   	
        packet = ServerReceive(sockfd, (struct sockaddr *)&clientaddr, &clientaddr_len, seqnum);
	printf("Writing to the file %s\n", packet.data);
	fwrite(packet.data, strlen(packet.data), 1, file);
	seqnum = !seqnum;
    } while (packet.header.len != 0);

    fclose(file);
    close(sockfd);
    return 0;
}
