/*
Name: Daniela Delatorre
Title: Lab 5-Stop and Wait for an Unreliable Channel
Description: This part of assignment is the client side of the UDP rdt2.2 protocol. It changes states from waiting for ACK 0 to waiting for ACK 1. If it sends packet 0 and receives ACK 0, then it sends the next packet and switches states and then waits for ACK 1.
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
#include <unistd.h>
#include <netdb.h>
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


int number = 0;
void ClientSend(int sockfd, const struct sockaddr *address, socklen_t addrlen, Packet packet) {
    while (1) {
	
	printf("%d\n", number);
	int random = (rand() % 11);
	printf("Random number is %d\n", random);
	if(random < 7){
		packet.header.seq_ack = number;
		printf("Sending the right sequence number\n");
	} else {
		packet.header.seq_ack = 50;
		printf("Sending the wrong sequence number\n");
	}

	packet.header.len = strlen(packet.data);
	if(random < 5){
		packet.header.cksum = getChecksum(packet);
		printf("Sending the correct checksum\n");
	} else {
		printf("Sending the wrong checksum\n");
	}

        // send the packet
        printf("Sending packet\n");
		//******STUDENT WORK******
	logPacket(packet);
	sendto(sockfd, &packet, sizeof(packet), 0, address, addrlen);
        // receive an ACK from the server
        Packet recvpacket;
		//******STUDENT WORK******
	recvfrom(sockfd, &recvpacket, sizeof(recvpacket), 0, address, &addrlen);
        // log what was received
        printf("Received ACK %d, checksum %d - ", recvpacket.header.seq_ack, recvpacket.header.cksum);

        // validate the ACK
        if (recvpacket.header.cksum != getChecksum(recvpacket)) {
            // bad checksum, resend packet
            printf("Bad checksum, expected checksum was: %d\n", getChecksum(recvpacket));
        } else if (recvpacket.header.seq_ack != packet.header.seq_ack) {
            // incorrect sequence number, resend packet
            printf("Bad seqnum, expected sequence number was: %d\n", packet.header.seq_ack);
        } else {
            // good ACK, we're done
            printf("Good ACK\n");
	    number = !number;
            break;
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

    // create and configure the socket [name the socket sockfd] 
    int sockfd;
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
    	perror("Error!\n");
	return 0;
    }


    // initialize the server address structure using argv[2] and argv[1]
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
	
    socklen_t length = sizeof(struct sockaddr);
    struct hostent *host;
    host = (struct hostent *)gethostbyname(argv[1]);
    address.sin_family = AF_INET;
    address.sin_port = htons(atoi(argv[2]));
    address.sin_addr = *((struct in_addr *)host->h_addr);

    // open file using argv[3]
    int file, byte;
    file = open(argv[3], O_RDWR);

    // send file contents to server
	//-create a packet
	//-set the seq_ack field 
	//-set the len field
	//-calculate the checksum
	//-send the packet
	//******STUDENT WORK******

    Packet pack;
    while((byte = read(file, pack.data, sizeof(pack.data))) > 0){
	//Reads from the file and then sends to the server
	ClientSend(sockfd, (struct sockaddr *)&address, length, pack);
	memset(&pack, 0, sizeof(pack));
    }

    pack.header.seq_ack = !number;
    pack.header.len = 0;
    pack.header.cksum = getChecksum(pack);
    pack.data[0] = '\0';

    // send zero-length packet to server to end connection
    ClientSend(sockfd, (struct sockaddr *)&address, length, pack);

    // clean up
    //-close the sockets and file
    close(file);
    close(sockfd);


    return 0;
}
