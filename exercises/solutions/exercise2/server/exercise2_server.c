/*
 ============================================================================
 Name        : esercizio2_server.c
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
#define PORT 48000

void clearwinsock()
{
#if defined WIN32
	WSACleanup();
#endif
}

void errorhandler(char *error_message)
{
	printf("%s", error_message);
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

	int my_socket;

	/* create a UDP socket */
	if ((my_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	{
		errorhandler("Error creating socket");
		clearwinsock();
		return EXIT_FAILURE;
	}

	/* set the server address */
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
	unsigned int client_address_length = sizeof(client_address);
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT);
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

	/* set server address */
	if ((bind(my_socket, (struct sockaddr *)&server_address,
			  sizeof(server_address))) < 0)
	{
		errorhandler("bind() failed");
		closesocket(my_socket);
		clearwinsock();
		return EXIT_FAILURE;
	}

	char buffer[BUFFMAX];
	int rcv_msg_size;
	while (1)
	{
		puts("\nServer listening...");
		/* clean buffer */
		memset(buffer, 0, BUFFMAX);
		/* receive message from client */
		if ((rcv_msg_size = recvfrom(my_socket, buffer, BUFFMAX, 0,
									 (struct sockaddr *)&client_address, &client_address_length)) < 0)
		{
			errorhandler("recvfrom() Helo failed");
			closesocket(my_socket);
			clearwinsock();
			return EXIT_FAILURE;
		}
		struct hostent *client_host = gethostbyaddr((char *)&client_address.sin_addr.s_addr, sizeof(client_address.sin_addr.s_addr), AF_INET);
		printf("Received '%s' from client %s (IP %s, port %d)\n", buffer, client_host->h_name, inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

		/* receive data from the client */
		memset(buffer, 0, BUFFMAX);
		if ((rcv_msg_size = recvfrom(my_socket, buffer, BUFFMAX, 0,
									 (struct sockaddr *)&client_address, &client_address_length)) < 0)
		{
			errorhandler("recvfrom() data failed");
			closesocket(my_socket);
			clearwinsock();
			return EXIT_FAILURE;
		}

		/* remove vowels from buffer and send it back to the client */
		int i, j;
		for (i = 0, j = 0; i < strlen(buffer); i++)
		{
			if (buffer[i] != 'a' && buffer[i] != 'e' && buffer[i] != 'i' && buffer[i] != 'o' && buffer[i] != 'u' &&
				buffer[i] != 'A' && buffer[i] != 'E' && buffer[i] != 'I' && buffer[i] != 'O' && buffer[i] != 'U' &&
				buffer[i] != '\n')
			{
				buffer[j] = buffer[i];
				j++;
			}
		}
		buffer[j] = '\0';
		printf("Sending '%s' back to client\n", buffer);
		if (sendto(my_socket, buffer, strlen(buffer), 0,
				   (struct sockaddr *)&client_address, sizeof(client_address)) != strlen(buffer))
		{
			errorhandler("sendto() sent a different number of bytes than expected");
			closesocket(my_socket);
			clearwinsock();
			return EXIT_FAILURE;
		}
	}
}

