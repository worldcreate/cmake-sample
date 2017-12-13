#include <stdio.h> //printf(), fprintf(), perror()
#include <sys/socket.h> //socket(), bind(), accept(), listen()
#include <arpa/inet.h> // struct sockaddr_in, struct sockaddr, inet_ntoa()
#include <stdlib.h> //atoi(), exit(), EXIT_FAILURE, EXIT_SUCCESS
#include <string.h> //memset()
#include <unistd.h> //close()

#include "./includes/func.h"

#define QUEUELIMIT 5

#define BUF_SIZE 32

int http(int sock) {
	char method[32];
	char buff[1024];
	char version[32];

	FILE *fp = fdopen(sock, "r");

	fscanf(fp, "%s %s %s\n", method, buff, version);
	printf("%s\n", method);

	fclose(fp);

	return EXIT_SUCCESS;
}

void spawn_fork(int clitSock, int servSock) {

	pid_t child_pid = fork();

	if (child_pid == -1) {
		perror("fork() failed.");
		exit(EXIT_FAILURE);
	} else if (child_pid == 0) {
		close(servSock);
		int ret = http(clitSock);
		close(clitSock);
		exit(ret);
	} else {
		close(clitSock);
	}
}

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
		clitLen = sizeof(clitSockAddr);
		if ((clitSock = accept(servSock, (struct sockaddr *) &clitSockAddr, &clitLen)) < 0) {
			perror("accept() failed.");
			exit(EXIT_FAILURE);
		}

		spawn_fork(clitSock, servSock);

	}

	close(servSock);

	return EXIT_SUCCESS;
}
