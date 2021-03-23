/*
*Program: lookup.c
*Function: Resolves IPv4 and IPv6 DNS for a website
*Arguments: filename website 
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define SOCKET int
#define GETSOCKETERRNO() (errno)
#ifndef AI_ALL
#define AI_ALL 0x0100
#endif

int main(int argc, char const *argv[])
{
	if(argc < 2) {
		printf("Usage\n\tlookup hostname");
		printf("Example:\n\tlookup example.com\n");
		exit(0);
	}

	printf("Resolving host name %s\n", argv[1]);
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = AI_ALL;
	struct addrinfo *peer_address;
	if(getaddrinfo(argv[1], 0, &hints, &peer_address)) {
		fprintf(stderr, "getaddrinfo failed %d\n", GETSOCKETERRNO());
		return 1;
	}
	//get name info turns getaddrinfo to text
	printf("Remote address is: \n");
	struct addrinfo *address = peer_address;
	do {
		char address_buffer[100];
		getnameinfo(address->ai_addr, address->ai_addrlen,
			address_buffer, sizeof(address_buffer),
			0,0, NI_NUMERICHOST);
		printf("\t%s\n", address_buffer);
	} while((address = address->ai_next));

	freeaddrinfo(peer_address);

	return 0;
}
