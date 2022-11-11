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
#include <ctype.h>
#include "protocol.h"

void clearwinsock() {
#if defined WIN32
	WSACleanup();
#endif
}

void errorhandler(char *errorMessage) {
	printf("%s", errorMessage);
}

int main(int argc, char *argv[]) {
#if defined WIN32
	// Initialize Winsock
	WSADATA wsa_data;
	int result = WSAStartup(MAKEWORD(2,2), &wsa_data);
	if (result != NO_ERROR) {
		printf("Error at WSAStartup()\n");
		return 0;
	}
#endif

	// create welcome socket
	int my_socket;
	my_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (my_socket < 0) {
		errorhandler("socket creation failed.\n");
		clearwinsock();
		return -1;
	}

	//set connection settings
	struct sockaddr_in sad;
	memset(&sad, 0, sizeof(sad)); // ensures that extra bytes contain 0 sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr("127.0.0.1");
	sad.sin_port = htons(PROTO_PORT); /* converts values between the host and network byte order. Specifically, htons() converts 16-bit quantities from host byte order to network byte order. */
	if (bind(my_socket, (struct sockaddr*) &sad, sizeof(sad)) < 0) {
		errorhandler("bind() failed.\n");
		closesocket(my_socket);
		clearwinsock();
		return -1;
	}

	// listen
	if (listen(my_socket, QLEN) < 0) {
		errorhandler("listen() failed.\n");
		closesocket(my_socket);
		clearwinsock();
		return -1;
	}

	// accept new connection
	struct sockaddr_in cad; // structure for the client address
	int client_socket;       // socket descriptor for the client
	int client_len;          // the size of the client address
	printf("Waiting for a client to connect...\n\n");
	while (1) {
		client_len = sizeof(cad); // set the size of the client address
		if ((client_socket = accept(my_socket, (struct sockaddr*) &cad,
				&client_len)) < 0) {
			errorhandler("accept() failed.\n");
			// close connection
			closesocket(client_socket);
			clearwinsock();
			return 0;
		}

		printf("Handling client %s\n", inet_ntoa(cad.sin_addr));

		char *s = "Connection established";
		if (send(client_socket, s, strlen(s), 0) != strlen(s)) {
			errorhandler(
					"send() sent a different number of bytes than expected");
			closesocket(client_socket);
			clearwinsock();
			return -1;
		}

		// get msg from client
		msg m;
		if ((recv(client_socket, &m, sizeof(msg), 0)) <= 0) {
			errorhandler("recv() failed or connection closed prematurely");
			closesocket(client_socket);
			clearwinsock();
			return -1;
		}
		printf("First string: %s\n", m.string1);
		printf("Second string: %s\n", m.string2);
		for (int i = 0; m.string1[i]; i++) {
			m.string1[i] = toupper(m.string1[i]);
		}
		for (int i = 0; m.string2[i]; i++) {
			m.string2[i] = tolower(m.string2[i]);
		}
		printf("First string to upper: %s\n", m.string1);
		printf("Second string to lower: %s\n", m.string2);
		// send manipulated data to client
		if (send(client_socket, &m, sizeof(msg), 0) != sizeof(msg)) {
			errorhandler(
					"send() sent a different number of bytes than expected");
			closesocket(client_socket);
			clearwinsock();
			return -1;
		}
	}

} // main end
