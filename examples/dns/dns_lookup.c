/*
 * getaddrinfo() / getnameinfo(): risoluzione diretta di "localhost"
 * e inversa di 127.0.0.1
 * Slide "5. Domain Name Service in C", pp. 9-11
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif

#define HOSTNAME_LEN 1025
int main(void) {
#if defined _WIN32
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
		printf("Error at WSAStartup()\n");
		return EXIT_FAILURE;
	}
#endif

	const char *name = "localhost";
	struct addrinfo hints, *res, *p;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;          /* solo IPv4 */
	hints.ai_socktype = SOCK_STREAM;    /* un risultato per indirizzo */

	int err = getaddrinfo(name, NULL, &hints, &res);
	if (err != 0) {
		fprintf(stderr, "getaddrinfo() failed: %s\n", gai_strerror(err));
		exit(EXIT_FAILURE);
	}
	for (p = res; p != NULL; p = p->ai_next) {
		char ip[INET_ADDRSTRLEN]; /* 16 = "255.255.255.255" + '\0' */
		getnameinfo(p->ai_addr, p->ai_addrlen, ip, sizeof(ip),
				NULL, 0, NI_NUMERICHOST);
		printf("Risultato di getaddrinfo(%s): %s\n", name, ip);
	}
	freeaddrinfo(res);

	struct sockaddr_in sa;
	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr("127.0.0.1");

	char host[HOSTNAME_LEN];
	err = getnameinfo((struct sockaddr *) &sa, sizeof(sa),
			host, sizeof(host), NULL, 0, NI_NAMEREQD);
	if (err != 0) {
		fprintf(stderr, "getnameinfo() failed: %s\n", gai_strerror(err));
		exit(EXIT_FAILURE);
	}
	printf("Risultato di getnameinfo(127.0.0.1): %s\n", host);
#if defined _WIN32
	WSACleanup();
#endif
	return EXIT_SUCCESS;
}
