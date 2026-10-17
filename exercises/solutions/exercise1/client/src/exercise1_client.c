#if defined _WIN32
#include <winsock2.h>
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

/* reads a line from stdin into s, removing the trailing newline;
 * characters that do not fit in s are discarded */
void read_line(const char *prompt, char *s, int size) {
	printf("%s", prompt);
	if (fgets(s, size, stdin) == NULL) {
		s[0] = '\0';
		return;
	}
	if (strchr(s, '\n') == NULL) {
		int c;
		while ((c = getchar()) != '\n' && c != EOF)
			;
	}
	s[strcspn(s, "\r\n")] = '\0';
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

	// create client socket
	int c_socket;
	c_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (c_socket < 0) {
		errorhandler("socket creation failed.\n");
		clearwinsock();
		return EXIT_FAILURE;
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
		return EXIT_FAILURE;
	}

	// receive the welcome message from server
	char buffer[BUFFER_SIZE];
	memset(buffer, '\0', BUFFER_SIZE);
	if ((recv(c_socket, buffer, BUFFER_SIZE - 1, 0)) <= 0) {
		errorhandler("recv() failed or connection closed prematurely\n");
		closesocket(c_socket);
		clearwinsock();
		return EXIT_FAILURE;
	}
	printf("%s\n", buffer);

	// get data from user
	msg m;
	memset(&m, '\0', sizeof(msg));
	read_line("Enter the first string: ", m.string1, BUFFER_SIZE);
	read_line("Enter the second string: ", m.string2, BUFFER_SIZE);

	// send data to server
	if (send(c_socket, (char*) &m, sizeof(msg), 0) != (int) sizeof(msg)) {
		errorhandler("send() sent a different number of bytes than expected\n");
		closesocket(c_socket);
		clearwinsock();
		return EXIT_FAILURE;
	}

	// get reply from server
	if (recv_all(c_socket, (char*) &m, sizeof(msg)) <= 0) {
		errorhandler("recv() failed or connection closed prematurely\n");
		closesocket(c_socket);
		clearwinsock();
		return EXIT_FAILURE;
	}
	m.string1[BUFFER_SIZE - 1] = '\0';
	m.string2[BUFFER_SIZE - 1] = '\0';
	printf("First string to upper: %s\n", m.string1);
	printf("Second string to lower: %s\n", m.string2);

	closesocket(c_socket);
	clearwinsock();
	return EXIT_SUCCESS;
} // main end
