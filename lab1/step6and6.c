/*
Name: Daniela Delatorre
Date: 09/24/2020
Title: Lab1-Unix/Linux Commmands and Overview of C Programming
Description: This part of the lab is an implementation of the quantitative comparisons between circuit switching and packet switching according to the following description:
	-The bandwidth of a network link is denoted by int linkBandwidth;
	-The bandwidth required for a given user is denoted by int userBandwidth;
	-The number of circuit switching users is denoted by int nCSusers;
	-The number of packet switching users is denoted by int nPSusers;
	-The percentage of time a packet switching user needs to transmit is denoted by double tPSuser;
	-The probability that a given (specific) packet switching user is busy transmitting is denoted by double pPSusersBusy;
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//factorial function that's meant to calculate and return the factorial of n
double factorial(double n){
	if(n==1){
		return 1;
	}
	else{
		return n*factorial(n-1);
	}
}

int main(){
	int i;
	//Values Given
	int linkBandwidth = 200;
	int userBandwidth = 20;
	int nCSusers;
	int nPSusers = 19;
	double tPSuser = 0.10;
	double pPSusersBusy;
	double pPSusersNotBusy;

	//Values Calculated
	nCSusers = linkBandwidth/ userBandwidth;

	//Step 6a
	printf("Step 6a\n");
	printf("The number of circuit switched users that can be computed is %d\n", nCSusers);

	//Step 7a
	printf("Step 7a\n");
	double pPSusers = tPSuser;
	printf("The Probability that a given specific user is busy transmitting is %f\n", pPSusers);

	//Step 7b
	printf("Step 7b\n");
	pPSusersNotBusy = 1 - pPSusers;
	printf("The probability that one specific other user is not busy is %f\n", pPSusersNotBusy);

	//Step 7c
	printf("Step 7c\n");
	double pAUnotbusy = pow((1-pPSusers), (nPSusers-1));
	printf("The probability that all other specific users are not busy is %f\n",pAUnotbusy);

	//Step 7d
	printf("Step 7d\n");
	double pOYON = (pPSusers*(pow(pPSusersNotBusy, (nPSusers-1))));
	printf("The probability that one specific user is transmitting and the remaining users are not transmitting is %f\n", pOYON);

	//Step 7e
	printf("Step 7e\n");
	double pOneBusy = (nPSusers*(pPSusers*(pow(pPSusersNotBusy, (nPSusers-1)))));
	printf("The probability that exactly one of users is busy is %f\n", pOneBusy);

	//Step 7f
	printf("Step 7f\n");
	double TenTransmitting = (pow(pPSusers, 10))*(pow(pPSusersNotBusy, (nPSusers-10)));
	printf("The probability that 10 specific users are transmitting and the others are idle is %f\n", TenTransmitting);

	//Step 7g
	printf("Step 7g\n");
	double AnyTenTransmitting = (factorial(nPSusers)/((factorial(10))*(factorial(nPSusers-10))))*(pow(pPSusers, 10))*(pow(pPSusersNotBusy, (nPSusers-10)));
	printf("The probability that any 10 users are transmitting and the others are idle is %f\n", AnyTenTransmitting);

	//Step 7h
	printf("Step 7h\n");
	double moreThanTenTransmitting = 0.0;
	for(i=11; i<nPSusers; i++){
		moreThanTenTransmitting += (factorial(nPSusers)/(factorial(i))*(factorial(nPSusers-i)))*(pow(pPSusers, i))*(pow(pPSusersNotBusy, (nPSusers-i)));
	}
	printf("The probability that more than 10 users are transmitting and the others are idle is %f\n", moreThanTenTransmitting);
 
	return 0;
}
