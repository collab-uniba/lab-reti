/*
 ============================================================================
 Name        : esercizio2_client.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#if defined WIN32
#include <winsock.h>
#else
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#define closesocket close
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NO_ERROR 0
#define BUFFMAX 255

void clearwinsock()
{
#if defined WIN32
	WSACleanup();
#endif
}

void errorhandler(const char *error_message)
{
	printf("%s\n", error_message);
}

int main(void)
{
#if defined WIN32
	// Initialize Winsock
	WSADATA wsa_data;
	int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (result != NO_ERROR)
	{
		printf("Error at WSAStartup()\n");
		return 0;
	}
#endif

	/* reads the name and port of the UDP server to contact (in the format srv.di.uniba.it:56700)*/
	char buffer[BUFFMAX];
	memset(buffer, 0, BUFFMAX);
	char *delim = ":";
	puts("Enter the server name and port (in the format servername:port):");
	fgets(buffer, BUFFMAX, stdin);
	/* split buffer into name:port */
	char *name = strtok(buffer, delim);
	int port = atoi(strtok(NULL, delim));
	struct hostent *host = gethostbyname(name);
	if (host == NULL)
	{
		errorhandler("Error getting host");
		clearwinsock();
		return EXIT_FAILURE;
	}
	printf("Server name: %s (IP: %s, port: %d)\n", name, inet_ntoa(*(struct in_addr *)host->h_addr), port);

	int my_socket;
	struct sockaddr_in server_address;
	unsigned int server_address_size = sizeof(server_address);
	int rcv_msg_size;

	/* create a UDP socket */
	if ((my_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	{
		errorhandler("Error creating socket");
		clearwinsock();
		return EXIT_FAILURE;
	}

	/* set the server address */
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	server_address.sin_addr = *((struct in_addr *)host->h_addr);

	char *message = "Helo";
	/* send a Helo message to the server */
	if (sendto(my_socket, message, strlen(message), 0,
			   (struct sockaddr *)&server_address, sizeof(server_address)) != strlen(message))
	{
		errorhandler("Error sending Helo");
		closesocket(my_socket);
		clearwinsock();
		return EXIT_FAILURE;
	}

	puts("Enter the string to send to the server:");
	memset(buffer, 0, BUFFMAX);
	fgets(buffer, BUFFMAX, stdin);
	/* send a message to the server */
	if (sendto(my_socket, buffer, strlen(buffer), 0,
			   (struct sockaddr *)&server_address, sizeof(server_address)) != strlen(buffer))
	{
		errorhandler("Error sending data");
		closesocket(my_socket);
		clearwinsock();
		return EXIT_FAILURE;
	}

	/* receive a message from the server */
	memset(buffer, 0, BUFFMAX);
	if ((rcv_msg_size = recvfrom(my_socket, buffer, BUFFMAX, 0,
								 (struct sockaddr *)&server_address, &server_address_size)) < 0)
	{
		errorhandler("Error receiving data");
		closesocket(my_socket);
		clearwinsock();
		return EXIT_FAILURE;
	}

	/* print the received message */
	printf("Received: %s", buffer);

	closesocket(my_socket);
	clearwinsock();
	return EXIT_SUCCESS;
}

