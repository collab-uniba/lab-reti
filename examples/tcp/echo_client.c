/*
 * Echo client TCP
 * Slide "2. Interazione (TCP) Client-Server", pp. 37-41
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
#include <stdlib.h>
#include <string.h> // for memset(), strlen()
#define BUFFERSIZE 512
#define PROTOPORT 57015 // Numero di porta di default
void errorhandler(char *error_message) {
	printf("%s",error_message);
}
void clearwinsock() {
#if defined _WIN32
	WSACleanup();
#endif
}

int main(void) {
#if defined _WIN32
	WSADATA wsa_data;
	int result = WSAStartup(MAKEWORD(2 ,2), &wsa_data);

	if (result != 0) {
		printf ("error at WSAStartup\n");
		return EXIT_FAILURE;
	}
#endif

	// CREAZIONE DELLA SOCKET
	int c_socket;
	c_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (c_socket < 0) {
		errorhandler("socket creation failed.\n");
		clearwinsock();
		return EXIT_FAILURE;
	}

	// COSTRUZIONE DELL’INDIRIZZO DEL SERVER
	struct sockaddr_in sad;
	memset(&sad, 0, sizeof(sad));
	sad.sin_family = AF_INET;
	sad.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP del server
	sad.sin_port = htons(PROTOPORT); // Server port

	// CONNESSIONE AL SERVER
	if (connect(c_socket, (struct sockaddr *)&sad, sizeof(sad))< 0) {
		errorhandler( "Failed to connect.\n" );
		closesocket(c_socket);
		clearwinsock();
		return EXIT_FAILURE;
	}

	char* input_string = "prova"; // Stringa da inviare
	int string_len = strlen(input_string); // Determina la lunghezza

	// INVIARE DATI AL SERVER
	if (send(c_socket, input_string, string_len, 0) != string_len) {
		errorhandler("send() failed or sent fewer bytes\n");
		closesocket(c_socket);
		clearwinsock();
		return EXIT_FAILURE;
	}

	// RICEVERE DATI DAL SERVER
	int bytes_rcvd;
	int total_bytes_rcvd = 0;
	char buf[BUFFERSIZE];   // buffer for data from the server
	memset(buf, 0, BUFFERSIZE); // ensures extra bytes contain 0
	printf("Received: ");   // Setup to print the echoed string

	while (total_bytes_rcvd < string_len) {
		if ((bytes_rcvd = recv(c_socket, buf + total_bytes_rcvd,
				BUFFERSIZE - 1 - total_bytes_rcvd, 0)) <= 0) {
			errorhandler("recv() failed or connection closed prematurely\n");
			closesocket(c_socket);
			clearwinsock();
			return EXIT_FAILURE;
		}
		total_bytes_rcvd += bytes_rcvd; // Keep tally of total bytes
	}
	buf[total_bytes_rcvd] = '\0'; // ridondante se si fa memset prima
	printf("%s", buf);    // Print the echo buffer

	// CHIUSURA DELLA CONNESSIONE
	closesocket(c_socket);
	clearwinsock();
	printf("\n");    // Print a final linefeed
	return EXIT_SUCCESS;
}
