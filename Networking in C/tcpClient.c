/*Run:      gcc tcpClient.c -o tcpClient.o
*Function:  Create TCP client capable of  sending HTTP reuests
*Use:       Run as ./tcpClient.o example.com, 80
*Data sent: GET/HTTP/1.1
			Host: example.com
*Output:    Returns HTML docuument
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

#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define SOCKET int 
#define GETSOCKETERRNO() (errno)



	//enables us to take command line arguments
int main(int argc, char const *argv[])
	{   //argc contains no. of arguments: Program name, hostname, port number
		//argv is actual values
		if(argc < 3){
			fprintf(stderr, "usage: tcp_client hostname/ %d\n",GETSOCKETERRNO());
			return 1;
		}

		printf("Configuring remote address...\n");
		struct addrinfo hints;
		memset(&hints, 0, sizeof(hints));
		//if UDP wanted, use SOCK_DGRAM
		hints.ai_socktype = SOCK_STREAM;
		struct addrinfo *peer_address;
		if (getaddrinfo(argv[1], argv[2], &hints, &peer_address)) {
			fprintf(stderr, "getaddrinfo() failed. %d\n ",GETSOCKETERRNO());
			return 1;
		}

		printf("remote address is: ");
		char address_buffer[100];
		char service_buffer[100];
		getnameinfo(peer_address->ai_addr, peer_address->ai_addrlen,
			address_buffer, sizeof(address_buffer),
			service_buffer, sizeof(service_buffer),
			NI_NUMERICHOST);
		printf("%s %s\n", address_buffer, service_buffer);

		printf("Creating socket\n");
		SOCKET socket_peer;
		socket_peer = socket(peer_address->ai_family,
			peer_address->ai_socktype,
			peer_address->ai_protocol);
		if(!ISVALIDSOCKET(socket_peer)) {
			fprintf(stderr, "socket failed. %d\n", GETSOCKETERRNO());
			return 1;
		}

		printf("Connecting...\n");
		if(connect(socket_peer, peer_address->ai_addr, peer_address->ai_addrlen)){
			fprintf(stderr, "connect failed. %d\n", GETSOCKETERRNO());
			return 1;
		}
		freeaddrinfo(peer_address);

		printf("Connected!\n");
		printf("Enter text to send data...\n");     

		while(1) {
			fd_set reads;
			FD_ZERO(&reads);
			FD_SET(socket_peer, &reads);


			struct timeval timeout;
			timeout.tv_sec = 0;
			timeout.tv_usec = 100000;

			if(select(socket_peer+1, &reads, 0,0, &timeout) < 0) {
				fprintf(stderr, "Selct failed. %d\n", GETSOCKETERRNO());
				return 1;
			}

			if(FD_ISSET(socket_peer, &reads)) {
				char read[4096];
				int bytes_received = recv(socket_peer, read, 4096, 0);
				if(bytes_received < 1) {
					printf("Connection closed by peer. \n");
					break;
				}
				printf("Received %d bytes: %*s", bytes_received, bytes_received,read);
			}

			char read[4096];
			if(!fgets(read, 4096, stdin)) break;
			printf("Sending %s\n", read);
			int bytes_sent = send(socket_peer, read, strlen(read), 0);
			printf("Sent %d bytes.\n", bytes_sent);
		}

		printf("CLosing socket\n");
		CLOSESOCKET(socket_peer);

		return 0;
	}
