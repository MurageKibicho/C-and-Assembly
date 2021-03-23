/*Program:  timeNetwork.c
*Run: 	   	gcc timeNetwork.c -o timeNetwork.o
*Function: 	Create server showing my computer's time
*Use: 		Run, open browser http://127.0.0.1:8080
*Output: 	Local time is:Sun Jun 21 03:58:48 2020
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
#include <time.h>

#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define SOCKET int 
#define GETSOCKETERRNO() (errno)

int main () {
	//Identify what local addres to bind to
	printf("Configuring local address...\n");
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET; //looking for IPv4
	hints.ai_socktype = SOCK_STREAM;//Sets TCP socket
	hints.ai_flags = AI_PASSIVE;//bind

	//hold info returned from gettaddrinfo()
	struct addrinfo *bind_address;
	getaddrinfo(0, "8080", &hints, &bind_address);//generates address

	printf("Creating socket...\n");
	SOCKET socket_listen;
	socket_listen = socket(bind_address->ai_family,
		bind_address->ai_socktype,
		bind_address->ai_protocol);

	if(!ISVALIDSOCKET(socket_listen)) {
		fprintf(stderr, "socket failed. %d\n", GETSOCKETERRNO());
		return 1;
	}

	printf("Binding socket to local addresses...\n");
	if(bind(socket_listen,
		bind_address->ai_addr, bind_address->ai_addrlen)) {
		fprintf(stderr, "bin failed. %d\n", GETSOCKETERRNO());
	return 1;
}
freeaddrinfo(bind_address);

printf("Listening...\n");
	//10 is how many connections it can queue
if(listen(socket_listen, 10) < 0) {
	fprintf(stderr, "listen failed. %d\n", GETSOCKETERRNO());
	return 1;
}

printf("Waiting for connection\n");
struct sockaddr_storage client_address;
socklen_t client_len = sizeof(client_address);

	//accepts blocks program till connection is made, creates new socket
SOCKET socket_client = accept(socket_listen,
	(struct sockaddr*) &client_address, &client_len);

if(!ISVALIDSOCKET(socket_client)) {
	fprintf(stderr, "Accept failed. %d\n", GETSOCKETERRNO());
	return 1;
}

printf("Client is connected..\n");
char address_buffer[100];
getnameinfo((struct sockaddr*) &client_address,
	client_len, address_buffer, sizeof(address_buffer), 0,0,
				 NI_NUMERICHOST);//specifies we want to see hostname as ip address

printf("%s\n", address_buffer);

printf("Reading request\n");
char request[1024];
int bytes_received =recv(socket_client, request, 1024, 0);
printf("Received %d bytes\n", bytes_received);


printf("Sending response...\n"); 	
const char *response = "HTTP/1.1 200 OK\r\n"
"COnnection: close\r\n"
"Content-type: text/plain\r\n\r\n"
"Local time is:";
int bytes_sent = send(socket_client, response,strlen(response),0);
printf("Sent %d of %d bytes.\n", bytes_sent, (int) strlen(response));

  	//get local time frome console and send
time_t timer;
time (&timer);
char *time_msg = ctime(&timer);
bytes_sent = send(socket_client, time_msg, strlen(time_msg), 0);
printf("Sent %d of %d bytes\n", bytes_sent, (int) strlen(time_msg));

  	//indicates we've sent all data
printf("CLosing connection...\n");
CLOSESOCKET(socket_client);

printf("CLosing listening socket...\n");
CLOSESOCKET(socket_listen);

printf("Finished\n");
return 0;

}