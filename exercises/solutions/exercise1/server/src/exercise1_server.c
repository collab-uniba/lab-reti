#if defined _WIN32
#include <winsock2.h>
#include <ws2tcpip.h> // socklen_t
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
#include <ctype.h>
#include "protocol.h"

void clearwinsock() {
#if defined _WIN32
	WSACleanup();
#endif
}

void errorhandler(char *errorMessage) {
	printf("%s", errorMessage);
}

/* TCP is a byte stream: a single recv() may return only part of the
 * message, so the fixed-size message is received in a loop */
int recv_all(int socket, char *buf, int len) {
	int total = 0;
	while (total < len) {
		int n = recv(socket, buf + total, len - total, 0);
		if (n <= 0) {
			return n;
		}
		total += n;
	}
	return total;
}

int main(void) {
#if defined _WIN32
	// Initialize Winsock
	WSADATA wsa_data;
	int result = WSAStartup(MAKEWORD(2,2), &wsa_data);
	if (result != 0) {
		printf("Error at WSAStartup()\n");
		return EXIT_FAILURE;
	}
#endif

	// create welcome socket
	int my_socket;
	my_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (my_socket < 0) {
		errorhandler("socket creation failed.\n");
		clearwinsock();
		return EXIT_FAILURE;
	}

	//set connection settings
	struct sockaddr_in sad;
	memset(&sad, 0, sizeof(sad)); // ensures that extra bytes contain 0
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr("127.0.0.1");
	sad.sin_port = htons(PROTO_PORT); /* converts values between the host and network byte order. Specifically, htons() converts 16-bit quantities from host byte order to network byte order. */
	if (bind(my_socket, (struct sockaddr*) &sad, sizeof(sad)) < 0) {
		errorhandler("bind() failed.\n");
		closesocket(my_socket);
		clearwinsock();
		return EXIT_FAILURE;
	}

	// listen
	if (listen(my_socket, QLEN) < 0) {
		errorhandler("listen() failed.\n");
		closesocket(my_socket);
		clearwinsock();
		return EXIT_FAILURE;
	}

	// accept new connection
	struct sockaddr_in cad; // structure for the client address
	int client_socket;       // socket descriptor for the client
	socklen_t client_len;          // the size of the client address
	printf("Waiting for a client to connect...\n\n");
	while (1) {
		client_len = sizeof(cad); // set the size of the client address
		if ((client_socket = accept(my_socket, (struct sockaddr*) &cad,
				&client_len)) < 0) {
			errorhandler("accept() failed.\n");
			// close the welcome socket
			closesocket(my_socket);
			clearwinsock();
			return EXIT_FAILURE;
		}

		printf("Handling client %s\n", inet_ntoa(cad.sin_addr));

		// send the welcome message
		char *s = WELCOME_MSG;
		int len = strlen(s);
		if (send(client_socket, s, len, 0) != len) {
			errorhandler("send() sent a different number of bytes than expected\n");
			closesocket(client_socket);
			continue; // keep serving other clients
		}

		// get msg from client
		msg m;
		if (recv_all(client_socket, (char*) &m, sizeof(msg)) <= 0) {
			errorhandler("recv() failed or connection closed prematurely\n");
			closesocket(client_socket);
			continue;
		}
		// make sure the strings are terminated
		m.string1[BUFFER_SIZE - 1] = '\0';
		m.string2[BUFFER_SIZE - 1] = '\0';
		printf("First string: %s\n", m.string1);
		printf("Second string: %s\n", m.string2);
		for (int i = 0; m.string1[i]; i++) {
			m.string1[i] = toupper((unsigned char) m.string1[i]);
		}
		for (int i = 0; m.string2[i]; i++) {
			m.string2[i] = tolower((unsigned char) m.string2[i]);
		}
		printf("First string to upper: %s\n", m.string1);
		printf("Second string to lower: %s\n", m.string2);
		// send manipulated data to client
		if (send(client_socket, (char*) &m, sizeof(msg), 0) != (int) sizeof(msg)) {
			errorhandler("send() sent a different number of bytes than expected\n");
		}
		// the client closes the connection; close our side and keep listening
		closesocket(client_socket);
	}
} // main end
