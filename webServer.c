#if defined(_WIN32)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <winsock2.h>
#include <ws2tcpip.h>

#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#endif

#if defined(_WIN32)
#define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)
#define CLOSESOCKET(s) closesocket(s)
#define GETSOCKETERRNO() (WSAGetLastError())
#else
#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define SOCKET int 
#define GETSOCKETERRNO() (errno)
#define TIMEOUT 5.0
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *get_content_type(const char* path) {
	const char *last_dot = strrchr(path, '.');
	if(last_dot) {
		if (strcmp(last_dot, ".css") == 0) return "text/css";
		if (strcmp(last_dot, ".csv") == 0) return "text/csv";
		if (strcmp(last_dot, ".gif") == 0) return "image/gif";
		if (strcmp(last_dot, ".htm") == 0) return "text/htm";
		if (strcmp(last_dot, ".html") == 0) return "text/html";
		if (strcmp(last_dot, ".ico") == 0) return "image/x-icon";
		if (strcmp(last_dot, ".jpeg") == 0) return "image/jpeg";
		if (strcmp(last_dot, ".jpg") == 0) return "image/jpg";
		if (strcmp(last_dot, ".js") == 0) return "application/js";
		if (strcmp(last_dot, ".json") == 0) return "application/json";
		if (strcmp(last_dot, ".png") == 0) return "image/png";
		if (strcmp(last_dot, ".pdf") == 0) return "application/pdf";
		if (strcmp(last_dot, ".svg") == 0) return "image/svg+xml";
		if (strcmp(last_dot, ".txt") == 0) return "text/txt";
	}
	return "application/octet-stream";
}

SOCKET create_socket(const char* host, const char *port) {
	printf("Configuring local address...\n");
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	struct addrinfo *bindAddress;
	getaddrinfo(host, port, &hints, &bindAddress);

	printf("Creating socket...\n");
	SOCKET socketListen;
	socketListen = socket(bindAddress->ai_family,
						   bindAddress->ai_socktype,
						   bindAddress->ai_protocol);
	if(!ISVALIDSOCKET(socketListen)) {
		fprintf(stderr, "socket failed. %d\n", GETSOCKETERRNO());
		exit(1);
	}

	printf("Binding socket to local address...\n");
	if(bind(socketListen,
			bindAddress->ai_addr,
			bindAddress->ai_addrlen)) {
		fprintf(stderr, "bind failed %d\n", GETSOCKETERRNO());
	exit(1);
	}
	freeaddrinfo(bindAddress);
	printf("Listening\n");
	if(listen(socketListen, 10) < 0){
		fprintf(stderr, "listen failed %d\n", GETSOCKETERRNO());
		exit(1);
	}
	return socketListen;
}
#define MAX_REQUEST_SIZE 2047
struct client_info {
	socklen_t addressLength;
	struct sockaddr_storage address;
	SOCKET socket;
	char request[MAX_REQUEST_SIZE + 1];
	int received;
	struct client_info *next;
};
static struct client_info *clients = 0;

struct client_info *get_client(SOCKET s) {
	struct client_info *ci = clients;

	while (ci) {
		if (ci->socket == s) 
			break;
			ci = ci->next;
		}

		if (ci) return ci;
		struct client_info *n =	(struct client_info*) calloc(1, sizeof(struct client_info));
		if(!n) {
			fprintf(stderr, "Out of memory\n");
			exit(1);
		}

		n->addressLength = sizeof(n->address);
		n->next = clients;
		clients = n;
		return n;	
}

void dropClient(struct client_info *client) {
	CLOSESOCKET(client->socket);

	struct client_info **p = &clients;

	while(*p) {
		if(*p == client) {
			*p = client->next;
			free(client);
			return;
		}
		p = &(*p)->next;
	}
	fprintf(stderr, "dropClient not found\n" );
	exit(1);
}

const char *get_client_address(struct client_info *ci) {
	static char addressBuffer[100];
	getnameinfo((struct sockaddr*) &ci->address,
				 ci->addressLength,
				 addressBuffer, sizeof(addressBuffer),
				 0, 0, NI_NUMERICHOST);
	return addressBuffer;
}

fd_set wait_on_client(SOCKET server) {
	fd_set reads;
	FD_ZERO(&reads);
	FD_SET(server, &reads);
	SOCKET maxSocket = server;

	struct client_info *ci = clients;
	while(ci) {
		FD_SET(ci->socket, &reads);
		if(ci->socket > maxSocket)
			maxSocket = ci->socket;
		ci = ci->next;
	}
	if(select(maxSocket + 1, &reads, 0, 0, 0) < 0) {
		fprintf(stderr, "select failed %d\n", GETSOCKETERRNO());
		exit(1);
	}
	return reads;
}

void send400(struct client_info *client) {
	const char	*c400 = "HTTP/1.1 400 Bad Request\r\n"
						"Connection: close\r\n"
						"Content-Length: 11\r\n\r\n Bad Request";
	send(client->socket, c400, strlen(c400), 0);
	dropClient(client);
}

void send404(struct client_info *client) {
	const char	*c404 = "HTTP/1.1 404 Not found\r\n"
						"Connection: close\r\n"
						"Content-Length: 9\r\n\r\n Not found";
	send(client->socket, c404, strlen(c404), 0);
	dropClient(client);
}

void serveResource(struct client_info *client, const char *path) {
	printf("Serve resource%s %s\n", get_client_address(client), path);

	if(strcmp(path, "/") == 0) path = "/index.html";

	if(strlen(path) > 100) {
		send400(client);
		return;
	}

	if(strstr(path, "..")) {
		send404(client);
		return;
	}

	char fullPath[128];
	sprintf(fullPath, "public%s", path);

	#if defined(_WIN32)
	char *p = fullPath;
	while (*p) {
		if (*p == '/') *p= "\\";
		p++;
	}
	#endif
	FILE *fp = fopen(fullPath, "rb");

	if(!fp) {
		send404(client);
		return;
	}
	fseek(fp, 0L, SEEK_END);
	size_t cl = ftell(fp);
	rewind(fp);

	const char *ct = get_content_type(fullPath);

	#define BSIZE 1024
		char buffer[BSIZE];
		sprintf(buffer, "HTTP/1.1 200 OK\r\n");
		send(client->socket, buffer, strlen(buffer),0);

		sprintf(buffer, "Connection close\r\n");
		send(client->socket, buffer, strlen(buffer),0);

		sprintf(buffer, "Content-Length %lu: \r\n", cl);
		send(client->socket, buffer, strlen(buffer),0);

		sprintf(buffer, "Content-Type %s\r\n", ct);
		send(client->socket, buffer, strlen(buffer),0);

		sprintf(buffer, "\r\n");
		send(client->socket, buffer, strlen(buffer),0);

		int r = fread(buffer, 1, BSIZE, fp);
		while(r) {
			send(client->socket, buffer, r, 0);
			r = fread(buffer, 1, BSIZE, fp);
		}
		fclose(fp);
		dropClient(client);
}
int main() {
	#if defined(_WIN32)
	WSADATA d;
	if(WSAStartup(MAKEWORD(2, 2), &d)) {
		fprintf(stderr, "Failed to initialize\n");
		return 1;
	}
	#endif

	SOCKET server = create_socket(0, "8080");

	while(1) {
		fd_set reads;
		reads = wait_on_client(server);
		if(FD_ISSET(server, &reads)) {
			struct client_info *client = get_client(-1);
			client->socket = accept(server,
									(struct sockaddr*)&(client->address),
									(socklen_t*)&(client->addressLength));

			if (!ISVALIDSOCKET(client->socket)) 
			{
				//fprintf(stderr, "Accept failed%d\n", GETSOCKETERRNO());
				perror("Error: ");
				return -1;
			}
			printf("New connection from %s\n", get_client_address(client));
		}
		struct client_info *client = clients;
		while(client) {
			struct client_info *next = client->next;
			if(FD_ISSET(client->socket, &reads)) {
				if (MAX_REQUEST_SIZE == client->received) {
					send400(client);
					client = next;
					continue;
				}
				int r = recv(client->socket,
							 client->request + client->received,
							 MAX_REQUEST_SIZE - client->received, 0);
				if(r < 1) {
					printf("Unexpected disconnect from %s\n", get_client_address(client));
					dropClient(client);
				} else {
					client->received += r;
					client->request[client->received] = 0;

					char *q = strstr(client->request, "\r\n\r\n");
					if(q) {
						if (strncmp("GET /", client->request, 5)) {
							send400(client);
						} else {
							char *path = client->request + 4;
							char *endPath = strstr(path, " ");
							if(!endPath) {
								send400(client);
							} else {
								*endPath = 0;
								serveResource(client, path);
							}
						}
					}
				}
			}
			client = next;
		}

	}

	printf("\n Closing socket\n");
	CLOSESOCKET(server);

	#if defined (_WIN32)
	WSACleanup();
	#endif

	printf("Finished\n");
	return 0;
}