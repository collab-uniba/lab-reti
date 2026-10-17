/*
 ============================================================================
 Name        : exercise2_client.c
 Description : Exercise 2 (UDP + DNS) - client
 ============================================================================
 */
#if defined _WIN32
#include <winsock2.h>
#include <ws2tcpip.h> // getaddrinfo(), getnameinfo(); link with ws2_32
#else
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

#define BUFFMAX 255

void clearwinsock()
{
#if defined _WIN32
	WSACleanup();
#endif
}

void errorhandler(const char *error_message)
{
	printf("%s\n", error_message);
}

int main(void)
{
#if defined _WIN32
	// Initialize Winsock
	WSADATA wsa_data;
	int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (result != 0)
	{
		printf("Error at WSAStartup()\n");
		return EXIT_FAILURE;
	}
#endif

	/* reads the name and port of the UDP server to contact (in the format srv.di.uniba.it:56700) */
	char buffer[BUFFMAX];
	memset(buffer, 0, BUFFMAX);
	puts("Enter the server name and port (in the format servername:port):");
	if (fgets(buffer, BUFFMAX, stdin) == NULL)
	{
		errorhandler("Error reading input");
		clearwinsock();
		return EXIT_FAILURE;
	}
	buffer[strcspn(buffer, "\r\n")] = '\0';
	/* split buffer into name:port */
	char *name = strtok(buffer, ":");
	char *port = strtok(NULL, ":");
	if (name == NULL || port == NULL)
	{
		errorhandler("Invalid format: expected servername:port");
		clearwinsock();
		return EXIT_FAILURE;
	}
	char server_name[BUFFMAX];
	strcpy(server_name, name);

	/* resolve name and port: with service != NULL, getaddrinfo() also sets the port */
	struct addrinfo hints, *res;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	int err = getaddrinfo(name, port, &hints, &res);
	if (err != 0)
	{
		printf("getaddrinfo() failed: %s\n", gai_strerror(err));
		clearwinsock();
		return EXIT_FAILURE;
	}
	struct sockaddr_in server_address;
	memcpy(&server_address, res->ai_addr, sizeof(server_address));
	freeaddrinfo(res);
	printf("Server name: %s (IP: %s, port: %d)\n", server_name,
		   inet_ntoa(server_address.sin_addr), ntohs(server_address.sin_port));

	int my_socket;
	/* create a UDP socket */
	if ((my_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	{
		errorhandler("Error creating socket");
		clearwinsock();
		return EXIT_FAILURE;
	}

	char *message = "Helo";
	int len = strlen(message);
	/* send a Helo message to the server */
	if (sendto(my_socket, message, len, 0,
			   (struct sockaddr *)&server_address, sizeof(server_address)) != len)
	{
		errorhandler("Error sending Helo");
		closesocket(my_socket);
		clearwinsock();
		return EXIT_FAILURE;
	}

	puts("Enter the string to send to the server:");
	memset(buffer, 0, BUFFMAX);
	if (fgets(buffer, BUFFMAX, stdin) == NULL)
	{
		buffer[0] = '\0';
	}
	buffer[strcspn(buffer, "\r\n")] = '\0';
	/* send a message to the server */
	len = strlen(buffer);
	if (sendto(my_socket, buffer, len, 0,
			   (struct sockaddr *)&server_address, sizeof(server_address)) != len)
	{
		errorhandler("Error sending data");
		closesocket(my_socket);
		clearwinsock();
		return EXIT_FAILURE;
	}

	/* receive a message from the server */
	struct sockaddr_in from_address;
	unsigned int from_address_size = sizeof(from_address);
	int rcv_msg_size;
	memset(buffer, 0, BUFFMAX);
	if ((rcv_msg_size = recvfrom(my_socket, buffer, BUFFMAX - 1, 0,
								 (struct sockaddr *)&from_address, &from_address_size)) < 0)
	{
		errorhandler("Error receiving data");
		closesocket(my_socket);
		clearwinsock();
		return EXIT_FAILURE;
	}
	buffer[rcv_msg_size] = '\0';

	/* print the received message with the server name and address */
	char ip[64];
	getnameinfo((struct sockaddr *)&from_address, sizeof(from_address), ip, sizeof(ip),
				NULL, 0, NI_NUMERICHOST);
	printf("Stringa '%s' ricevuta dal server nome: %s indirizzo: %s\n", buffer, server_name, ip);

	closesocket(my_socket);
	clearwinsock();
	return EXIT_SUCCESS;
}
