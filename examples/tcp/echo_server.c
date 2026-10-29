/*
 * Echo server TCP
 * Slide "2. Interazione (TCP) Client-Server", pp. 25-28
 */
#if defined _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h> // for atoi()
#include <string.h> // for memset(), strlen()
#define PROTOPORT 57015 // default protocol port number
#define QLEN 6 // size of request queue
#define BUFFERSIZE 512 // size of the buffer
void errorhandler(char *errorMessage) {
	printf ("%s", errorMessage);
}
void clearwinsock() {
#if defined _WIN32
	WSACleanup();
#endif
}

int main(int argc, char *argv[]) {
	int port;
	if (argc > 1) {
		port = atoi(argv[1]); // if argument specified convert
		                      // argument to binary
	}
	else
		port = PROTOPORT; // use default port number
	if (port <= 0 || port > 65535) {
		printf("bad port number %s \n", argv[1]);
		return EXIT_FAILURE;
	}

#if defined _WIN32 // initialize Winsock
	WSADATA wsa_data;
	int result = WSAStartup(MAKEWORD(2,2), &wsa_data);

	if (result != 0) {
		errorhandler("Error at WSAStartup()\n");
		return EXIT_FAILURE;
	}
#endif

	// CREAZIONE DELLA SOCKET
	int my_socket;
	my_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (my_socket < 0) {
		errorhandler("socket creation failed.\n");
		clearwinsock();
		return EXIT_FAILURE;
	}

	// ASSEGNAZIONE DI UN INDIRIZZO ALLA SOCKET
	struct sockaddr_in sad;
	memset(&sad, 0, sizeof(sad)); // ensures that extra bytes contain 0
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr("127.0.0.1");
	sad.sin_port = htons(port); /* converts values between the host and network byte order. Specifically, htons() converts 16-bit quantities from host byte order to network byte order. */
	if (bind(my_socket, (struct sockaddr*) &sad, sizeof(sad)) < 0) {
		errorhandler("bind() failed.\n");
		closesocket(my_socket);
		clearwinsock();
		return EXIT_FAILURE;
	}

	// SETTAGGIO DELLA SOCKET ALL'ASCOLTO
	if (listen (my_socket, QLEN) < 0) {
		errorhandler("listen() failed.\n");
		closesocket(my_socket);
		clearwinsock();
		return EXIT_FAILURE;
	}
	// ACCETTARE UNA NUOVA CONNESSIONE
	struct sockaddr_in cad; // structure for the client address
	int client_socket;       // socket descriptor for the client
	socklen_t client_len;    // the size of the client address
	printf("Waiting for a client to connect...\n");
	while (1) { /* oppure for (;;) */
		client_len = sizeof(cad); // set the size of the client address
		if ((client_socket = accept(my_socket, (struct sockaddr *)&cad,
				&client_len)) < 0) {
			errorhandler("accept() failed.\n");
			closesocket(my_socket);
			clearwinsock();
			return EXIT_FAILURE;
		}
		printf("Handling client %s\n", inet_ntoa(cad.sin_addr));
		// ECHO DELLA STRINGA RICEVUTA E CHIUSURA DELLA CONNESSIONE
		char buf[BUFFERSIZE];
		int bytes_rcvd = recv(client_socket, buf, BUFFERSIZE, 0);
		if (bytes_rcvd > 0) send(client_socket, buf, bytes_rcvd, 0);
		closesocket(client_socket);
	} // end-while
} // end-main
