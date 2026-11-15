/*
 * Echo client UDP
 * Slide "4. Socket UDP", pp. 12-15
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
#include <stdlib.h> /* for exit(), EXIT_FAILURE */
#include <string.h> /* for memset() */

#define ECHOMAX 255
#define PORT 48000

void ErrorHandler(char *errorMessage) {
	printf("%s", errorMessage);
}

void ClearWinSock() {
#if defined _WIN32
	WSACleanup();
#endif
}

int main() {
#if defined _WIN32
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2 ,2), &wsaData);
	if (iResult != 0) {
		printf ("error at WSAStartup\n");
		return EXIT_FAILURE;
	}
#endif

	int sock;
	struct sockaddr_in echoServAddr;
	struct sockaddr_in fromAddr;
	socklen_t fromSize;
	char echoString[ECHOMAX+1];
	char echoBuffer[ECHOMAX+1];
	int echoStringLen;
	int respStringLen;

	printf("Inserisci la stringa echo da inviare al server\n");
	scanf("%255s", echoString);
	if ((echoStringLen = strlen(echoString)) > ECHOMAX) {
		ErrorHandler("echo word too long\n");
		ClearWinSock(); return EXIT_FAILURE;
	}

	// CREAZIONE DELLA SOCKET
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		ErrorHandler("socket() failed\n");
		ClearWinSock(); return EXIT_FAILURE;
	}

	// COSTRUZIONE DELL'INDIRIZZO DEL SERVER
	memset(&echoServAddr, 0, sizeof(echoServAddr));
	echoServAddr.sin_family = AF_INET;
	echoServAddr.sin_port = htons(PORT);
	echoServAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	// INVIO DELLA STRINGA ECHO AL SERVER
	if (sendto(sock, echoString, echoStringLen, 0,
			(struct sockaddr*)&echoServAddr, sizeof(echoServAddr))
			!= echoStringLen) {
		ErrorHandler("sendto() sent wrong number of bytes\n");
		closesocket(sock); ClearWinSock(); return EXIT_FAILURE;
	}

	// RITORNO DELLA STRINGA ECHO
	fromSize = sizeof(fromAddr);
	respStringLen = recvfrom(sock, echoBuffer, ECHOMAX, 0,
			(struct sockaddr*)&fromAddr, &fromSize);

	if (respStringLen < 0) {
		ErrorHandler("recvfrom() failed\n");
		closesocket(sock);
		ClearWinSock();
		return EXIT_FAILURE;
	}
	if (echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr) {
		fprintf(stderr, "Error: received a packet from unknown source.\n");
		closesocket(sock); ClearWinSock(); return EXIT_FAILURE;
	}

	echoBuffer[respStringLen] = '\0';
	printf("Received: %s\n", echoBuffer);

	closesocket(sock);
	ClearWinSock();
	return EXIT_SUCCESS;
}
