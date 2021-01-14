/*
Name: Daniela Delatorre
Date: October 5 2020
Title: Lab2 - Multithreading in C and network commands
Description: 	Calculates:
			the time that elapses from when a client clicks on a link until they receive the object,
			the time that elapses from when a client clicks on a link until they receive the base object and 6 additional objects
			the time that elapses for n persistent, parallel TCP connections
			the time that elapses for n non-persistent parallel TCP connections
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	//converts the string comand line argument to an integer
	int n = atoi(argv[1]);
	int RTT0 = 3;
	int RTT1 = 20;
	int RTT2 = 26;
	int RTTHTTP = 47;
	//total DNS lookup time is RTT0 + RTT1 + RTT2
	int RTTDNS = RTT0 + RTT1 + RTT2;
	int i;
	
	//need to add the time it takes for the DNS lookup, the RTT to initiate the TCP connection, and one RTT for the HTTP request
	printf("One object takes %d milliseconds\n", RTTDNS + 2*RTTHTTP);

	//need to add the time for DNS lookup, base TCP connection and get request, and TCP, get request for each object
	printf("6 Non persistent objects takes %d milliseconds\n", RTTDNS + 2*RTTHTTP + 2*6*RTTHTTP);

	//To calculate the number of groups of parallel connections, need to set np to 1 if n is greater is not, round up to get the number of groups
	int np = (n>6)? 1: 6/n;
	if((n<6) && (6%n != 0)){
		np += 1;
	}

	//total time is the DNS lookup + base TCP connection and get request + number of groups*time it takes to make get request
	printf("%d parallel persistent TCP connections takes %d milliseconds\n", n, RTTDNS + 2*RTTHTTP + np*RTTHTTP);
	//total time is the DNS lookup + base TCP connection and get request + 2(one TCP connection the other for get request)*number of groups*time it takes to do HTTP request
	printf("%d parallel non-persistent TCP connections takes %d milliseconds\n", n, RTTDNS + 2*RTTHTTP + 2*np*RTTHTTP);

	return 0;
}
