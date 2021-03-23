/*
*Program:  listNetworkAdapters.c
*Run: 	   gcc listNetworkAdapters.c -o listNetworkAdapters.o
*Function: Simple program to list IPv4 and IPv6 adapters on linux computer
*
*Expected output: 		lo	IPv4		127.0.0.1 		//computer address
*					wlo1	IPv4		192.168.1.9 //router address
*					lo	IPv6		::1
*					wlo1	IPv6		fe80::c9cd:3159:934:170e%wlo1
*/
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
	//declare variable to store addresses
	struct ifaddrs *addresses;

	//getifaddrs() allocates memory and fills linked list with addresses
	if (getifaddrs(&addresses) == -1) {
		printf("getifaddrs call failed\n");
		return -1; //return 0 on success, -1 if failed
	}

	//New pointer to walk through linked list of addresses
	struct ifaddrs *address = addresses;

	//loop gets next address by calling address->ifa_next
	while(address) {
		//Identify address family: AF_INET = IPv4 ?AF_INET6 = IPv6
		int family = address->ifa_addr->sa_family;

		if(family == AF_INET || family == AF_INET6) {
			//print address families
			printf("%s\t", address->ifa_name);
			printf("%s\t", family == AF_INET ? "IPv4" : "IPv6");
			//character buffer to store textual addresses
			char ap[100];

			const int family_size = family == AF_INET ? sizeof(struct sockaddr_in) : 
			sizeof(struct sockaddr_in6);
		//getnameinfo fills buffer											
			getnameinfo(address->ifa_addr,family_size, ap, sizeof(ap),
				0, 0, NI_NUMERICHOST);
			printf("\t%s\n", ap);
		}
		address = address->ifa_next;
	}

	//free memory allocated by getifaddrs()
	freeifaddrs(addresses);
	return 0;
}