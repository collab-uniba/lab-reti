/*
 ============================================================================
 Name        : exercise2_server.c
 Description : Exercise 2 (UDP + DNS) - server
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
#define PORT 56700

void clearwinsock()
{
#if defined _WIN32
	WSACleanup();
#endif
}

void errorhandler(char *error_message)
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
	unsigned int client_address_length;
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
		fflush(stdout);
		/* receive the Helo message from client */
		memset(buffer, 0, BUFFMAX);
		client_address_length = sizeof(client_address);
		if ((rcv_msg_size = recvfrom(my_socket, buffer, BUFFMAX - 1, 0,
									 (struct sockaddr *)&client_address, &client_address_length)) < 0)
		{
			errorhandler("recvfrom() Helo failed");
			continue;
		}
		buffer[rcv_msg_size] = '\0';

		/* client name (reverse lookup) and address: without NI_NAMEREQD,
		   getnameinfo() returns the numeric address if the name does not exist;
		   if it fails anyway (e.g. resolver error), fall back to inet_ntoa() */
		char client_name[1025], client_ip[64];
		if (getnameinfo((struct sockaddr *)&client_address, sizeof(client_address),
						client_ip, sizeof(client_ip), NULL, 0, NI_NUMERICHOST) != 0)
		{
			strcpy(client_ip, inet_ntoa(client_address.sin_addr));
		}
		if (getnameinfo((struct sockaddr *)&client_address, sizeof(client_address),
						client_name, sizeof(client_name), NULL, 0, 0) != 0)
		{
			strcpy(client_name, client_ip);
		}
		printf("Ricevuti dati dal client nome: %s, indirizzo: %s\n", client_name, client_ip);

		/* receive data from the client */
		memset(buffer, 0, BUFFMAX);
		client_address_length = sizeof(client_address);
		if ((rcv_msg_size = recvfrom(my_socket, buffer, BUFFMAX - 1, 0,
									 (struct sockaddr *)&client_address, &client_address_length)) < 0)
		{
			errorhandler("recvfrom() data failed");
			continue;
		}
		buffer[rcv_msg_size] = '\0';

		/* remove vowels from buffer and send it back to the client */
		char reply[BUFFMAX];
		int j = 0;
		for (int i = 0; buffer[i] != '\0'; i++)
		{
			if (strchr("aeiouAEIOU\r\n", buffer[i]) == NULL)
			{
				reply[j++] = buffer[i];
			}
		}
		reply[j] = '\0';
		printf("Ricevuto '%s', inviato '%s'\n", buffer, reply);
		fflush(stdout);
		if (sendto(my_socket, reply, j, 0,
				   (struct sockaddr *)&client_address, sizeof(client_address)) != j)
		{
			errorhandler("sendto() sent a different number of bytes than expected");
		}
	}
}
