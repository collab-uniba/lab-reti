/*
 * Risolve il nome o l'indirizzo IPv4 passato da riga di comando
 * Slide "5. Domain Name Service in C", p. 12 ("Altro Esempio");
 * include e inizializzazione di Winsock come a p. 9
 *
 * Uso: resolve <hostname | ipv4 address>
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
int main(int argc, char **argv) {
	if (argc != 2) {
		printf("usage: %s hostname | ipv4 address\n", argv[0]);
		return EXIT_FAILURE;
	}
	/* inizializzazione di Winsock su Windows */
#if defined _WIN32
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
		printf("Error at WSAStartup()\n");
		return EXIT_FAILURE;
	}
#endif
	struct addrinfo hints, *res;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	/* getaddrinfo() accetta sia un nome sia un indirizzo numerico */
	int err = getaddrinfo(argv[1], NULL, &hints, &res);
	if (err != 0) {
		fprintf(stderr, "getaddrinfo() failed: %s\n", gai_strerror(err));
		return EXIT_FAILURE;
	}
	char ip[INET_ADDRSTRLEN], host[HOSTNAME_LEN];
	getnameinfo(res->ai_addr, res->ai_addrlen, ip, sizeof(ip),
			NULL, 0, NI_NUMERICHOST);
	if (getnameinfo(res->ai_addr, res->ai_addrlen, host, sizeof(host),
			NULL, 0, NI_NAMEREQD) != 0)
		strcpy(host, "(non disponibile)");
	printf("%s -> indirizzo %s, nome %s\n", argv[1], ip, host);
	freeaddrinfo(res);
#if defined _WIN32
	WSACleanup();
#endif
	return EXIT_SUCCESS;
}
