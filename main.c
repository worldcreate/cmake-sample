#include <stdio.h> //printf(), fprintf(), perror()
#include <sys/socket.h> //socket(), bind(), accept(), listen()
#include <arpa/inet.h> // struct sockaddr_in, struct sockaddr, inet_ntoa()
#include <stdlib.h> //atoi(), exit(), EXIT_FAILURE, EXIT_SUCCESS
#include <string.h> //memset()
#include <unistd.h> //close()

#include "./includes/func.h"

#define QUEUELIMIT 5

int main(int argc, char** argv) {

	int servSock; //server socket descripter
	int clitSock; //client socket descripter
	struct sockaddr_in servSockAddr; //server internet socket address
	struct sockaddr_in clitSockAddr; //client internet socket address
	unsigned short servPort; //server port number
	unsigned int clitLen; // client internet socket address length
	int yes = 1;

	if ( argc != 2) {
		fprintf(stderr, "argument count mismatch error.\n");
		exit(EXIT_FAILURE);
	}

	if ((servPort = (unsigned short) atoi(argv[1])) == 0) {
		fprintf(stderr, "invalid port number.\n");
		exit(EXIT_FAILURE);
	}

	if ((servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 ){
		perror("socket() failed.");
		exit(EXIT_FAILURE);
	}

	memset(&servSockAddr, 0, sizeof(servSockAddr));
	servSockAddr.sin_family	  = AF_INET;
	servSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servSockAddr.sin_port		= htons(servPort);

	setsockopt(servSock, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes, sizeof(yes));

	if (bind(servSock, (struct sockaddr *) &servSockAddr, sizeof(servSockAddr) ) < 0 ) {
		perror("bind() failed.");
		exit(EXIT_FAILURE);
	}

	if (listen(servSock, QUEUELIMIT) < 0) {
		perror("listen() failed.");
		exit(EXIT_FAILURE);
	}

	LOG("test");

	while(1) {
		char buff[1024];
		clitLen = sizeof(clitSockAddr);
		if ((clitSock = accept(servSock, (struct sockaddr *) &clitSockAddr, &clitLen)) < 0) {
			perror("accept() failed.");
			exit(EXIT_FAILURE);
		}

		int size = 0;
		while ((size = read(clitSock , buff, 32)) > 0) {
			printf("size = %d\n", size);
			printf("%s", buff);
			memset(buff, 0, sizeof(buff));
			if (size < 32) {
				break;
			}
			printf("\n");
		}

		printf("\n\nconnected from %s.\n", inet_ntoa(clitSockAddr.sin_addr));
	  close(clitSock);
	}

	return EXIT_SUCCESS;

	return 0;
}
