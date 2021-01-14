/*
Name: Daniela Delatorre
Date: November 2, 2020
Title: Lab6-Stop and Wait for an Unreliable Channel, with Loss
Description: This lab is very similar to lab 5 except that the client side includes a timer, so that whenever the ACK gets lost from the server side, after 10 seconds, it will resend the packet instead of waiting forever to receive the ACK. It includes the same states as those in Lab 5.
*/

//Explain what each library does in the context of this lab. Do we need all of them?
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <time.h>
#include <unistd.h>
#include "lab6.h"

#define PLOSTMSG 11
//add the missing functions

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

void ServerSend(int sockfd, const struct sockaddr *address, socklen_t addrlen, int seqnum) {
    Packet packet;
    int random = rand()%11;
    printf("Random is %d", random);
    if(random < 7){
	packet.header.seq_ack = seqnum;
	printf("sending the right seq\n");
    } else {
	packet.header.seq_ack = 50;
	printf("sending the wrong seq\n");
    }
    packet.header.len = 0;
    if(random < 5){
	packet.header.cksum = getChecksum(packet);
	printf("sending the right ceckk\n");
    } else {
	printf("sending the wrong check\n");
    }
    // Simulatting a chance that ACK gets lost
    if (rand() % PLOSTMSG == 0) {
        printf("Packet is dropped\n");
    } else {
	sendto(sockfd, &packet, sizeof(packet), 0, address, addrlen);
	printf("Sent ACK: %d and CheckSum: %d\n", packet.header.seq_ack,packet.header.cksum);
    }

    printf("Sent ACK %d, checksum %d\n", packet.header.seq_ack, packet.header.cksum);
}

Packet ServerReceive(int sockfd, struct sockaddr *address, socklen_t *addrlen, int seqnum)
{
                     
    Packet packet;

    while (1) {
//        Receive a packet from the client
        printf("Sequence Number: %d\n", seqnum);
	recvfrom(sockfd, &packet, sizeof(packet), 0, address, addrlen);

        // validate the length of the packet
        //STUDENT WORK

        // log what was received
        printf("Received: ");
        logPacket(packet);

        //verify the checksum and the sequence number
        if (packet.header.cksum != getChecksum(packet)) {
            printf("Bad checksum, expected %d\n", getChecksum(packet));
            ServerSend(sockfd, address, *addrlen, !seqnum);
        } else if (packet.header.seq_ack != seqnum) {
            printf("Bad seqnum, expected %d\n", seqnum);
            ServerSend(sockfd, address, *addrlen, !seqnum);
        } else {
            printf("Good packet\n");
            ServerSend(sockfd, address, *addrlen, seqnum);
	    printf("Writing to the file %s\n", packet.data);
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
	perror("Failed to create the socket\n");
	exit(1);
    }

    // initialize the server address structure
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(atoi(argv[1]));
    address.sin_addr.s_addr = INADDR_ANY;

    // bind the socket
    if((bind(sockfd, (struct sockaddr *)&address, sizeof(struct sockaddr))) < 0){
	perror("Failed to bind the socket\n");
	exit(1);
    }

    //Please answer the following questions
        //[Q1] Briefly explain what is "AF_INET" used for.
        //[Q2] Briefly explain what is "SOCK_DGRAM" used for.
        //[Q3] Briefly explain what is "htons" used for.
        //[Q4] Briefly explain why bind is required on the server and not on the client.
    
    
    
    // open file
    FILE *file = fopen(argv[2], "wb");
    if (file == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    // get file contents from client and save it to the file
    int seqnum = 0;
    Packet packet;
    struct sockaddr_in clientaddr;
    socklen_t clientaddr_len = sizeof(clientaddr);
    do {
        packet = ServerReceive(sockfd, (struct sockaddr *)&clientaddr, &clientaddr_len, seqnum);
	printf("Writing to the file %s\n", packet.data);
	fwrite(packet.data, 1, strlen(packet.data), file);
	seqnum = !seqnum;
    } while (packet.header.len != 0);

    //cleanup
    fclose(file);
    close(sockfd);
    
    return 0;
}
