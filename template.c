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
//...

int main(int argc, char *argv[]) {
	//...
#if defined WIN32
	// Initialize Winsock
	WSADATA wsa_data;
	int result =
	WSAStartup(MAKEWORD(2,2), &wsa_data);
	if (result != 0) {
		printf("Error at WSAStartup()\n");
		return 0;
	}
#endif
	int my_socket;

	//...

	closesocket(my_socket);
#if defined WIN32
	WSACleanup();
#endif
	return 0;
} // main end
