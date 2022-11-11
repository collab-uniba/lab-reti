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

	// create client socket
	int c_socket;
	c_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (c_socket < 0) {
		errorhandler("socket creation failed.\n");
		closesocket(c_socket);
		clearwinsock();
		return -1;
	}

	// set connection settings
	struct sockaddr_in sad;
	memset(&sad, 0, sizeof(sad));
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP del server
	sad.sin_port = htons(PROTO_PORT); // Server port

	// connection
	if (connect(c_socket, (struct sockaddr*) &sad, sizeof(sad)) < 0) {
		errorhandler("Failed to connect.\n");
		closesocket(c_socket);
		clearwinsock();
		return -1;
	}

	// receive from server
	char buffer[BUFFER_SIZE];
	memset(buffer, '\0', BUFFER_SIZE);
	if ((recv(c_socket, buffer, BUFFER_SIZE - 1, 0)) <= 0) {
		errorhandler("recv() failed or connection closed prematurely");
		closesocket(c_socket);
		clearwinsock();
		return -1;
	}
	printf("%s\n", buffer); // Print the echo buffer

	// get data from user
	char string1[BUFFER_SIZE];
	memset(string1, '\0', BUFFER_SIZE);
	printf("Enter the first string: ");
	scanf("%s", string1);
	char string2[BUFFER_SIZE];
	memset(string2, '\0', BUFFER_SIZE);
	printf("Enter the second string: ");
	scanf("%s", string2);
	msg m;
	strcpy(m.string1, string1);
	strcpy(m.string2, string2);

	// send data to server
	if (send(c_socket, &m, sizeof(msg), 0) != sizeof(msg)) {
		errorhandler("send() sent a different number of bytes than expected");
		closesocket(c_socket);
		clearwinsock();
		return -1;
	}

	// get reply from server
	if ((recv(c_socket, &m, sizeof(msg), 0)) <= 0) {
		errorhandler("recv() failed or connection closed prematurely");
		closesocket(c_socket);
		clearwinsock();
		return -1;
	}
	printf("First string to upper: %s\n", m.string1);
	printf("Second string to lower: %s\n", m.string2);

	closesocket(c_socket);
	clearwinsock();
	return 0;
} // main end
