/*
 * Echo server UDP
 * Slide "4. Socket UDP", pp. 9-11
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
	struct sockaddr_in echoClntAddr;
	socklen_t cliAddrLen;
	char echoBuffer[ECHOMAX+1];
	int recvMsgSize;

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

	// BIND DELLA SOCKET
	if ((bind(sock, (struct sockaddr *)&echoServAddr, sizeof(echoServAddr))) < 0) {
		ErrorHandler("bind() failed\n");
		closesocket(sock); ClearWinSock(); return EXIT_FAILURE;
	}

	// RICEZIONE DELLA STRINGA ECHO DAL CLIENT
	while(1) {
		cliAddrLen = sizeof(echoClntAddr);
		recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0,
				(struct sockaddr*)&echoClntAddr, &cliAddrLen);
		if (recvMsgSize < 0) { ErrorHandler("recvfrom() failed\n"); continue; }
		echoBuffer[recvMsgSize] = '\0'; // termina la stringa ricevuta

		printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));
		printf("Received: %s\n", echoBuffer);
		fflush(stdout);

		// RINVIA LA STRINGA ECHO AL CLIENT
		if (sendto(sock, echoBuffer, recvMsgSize, 0, (struct sockaddr *)&echoClntAddr,
				sizeof(echoClntAddr)) != recvMsgSize)
			ErrorHandler("sendto() sent different number of bytes than expected\n");
	}
} // end-main
