#if defined _WIN32
#include <winsock2.h>
#include <ws2tcpip.h> // getaddrinfo(), getnameinfo(); link with ws2_32
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
//...

void clearwinsock() {
#if defined _WIN32
	WSACleanup();
#endif
}

int main(int argc, char *argv[]) {
	//...
#if defined _WIN32
	// Initialize Winsock
	WSADATA wsa_data;
	int result = WSAStartup(MAKEWORD(2,2), &wsa_data);
	if (result != 0) {
		printf("Error at WSAStartup()\n");
		return EXIT_FAILURE;
	}
#endif
	int my_socket;

	//...

	closesocket(my_socket);
	clearwinsock();
	return EXIT_SUCCESS;
} // main end
