#include <asm-generic/socket.h>
#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "tcp.h"

void sigchld_handler(int s)
{
	//waitpid() might overwrite errno, so we save and restore it:
	int saved_errno = errno;

	while (waitpid(-1, NULL, WNOHANG) > 0)
		;

	errno = saved_errno;
}

void* get_in_addr(struct sockaddr* sa)
{
	if (sa->sa_family == AF_INET)
		return &(((struct sockaddr_in*)sa)->sin_addr);
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void initialize_servaddrinfo(struct addrinfo** servinfo)
{
	struct addrinfo hints;
	int err;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	err = getaddrinfo(NULL, PORT, &hints, servinfo);

	if (err != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
		exit(EXIT_FAILURE);
	}
}

void setup_socket(struct addrinfo* servinfo, int* sockfd)
{
	struct addrinfo* p;
	int err;
	int yes = 1;

	for (p = servinfo; p != NULL; p = p->ai_next) {
		(*sockfd) = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if ((*sockfd) == -1) {
			perror("server: socket");
			continue;
		}

		err = setsockopt((*sockfd), SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
		if (err == -1) {
			perror("setsockopt");
			exit(EXIT_FAILURE);
		}

		err = bind((*sockfd), p->ai_addr, p->ai_addrlen);
		if (err == -1) {
			close((*sockfd));
			perror("server: bind");
			continue;
		}

		break;
	}

	freeaddrinfo(servinfo);

	if (p == NULL) {
		fprintf(stderr, "server: failed to bind\n");
		exit(EXIT_FAILURE);
	}
}

void setup_tcp_server(int* sockfd)
{
	struct addrinfo* servinfo;
	struct sigaction sa;

	int err;

	initialize_servaddrinfo(&servinfo);

	setup_socket(servinfo, sockfd);

	err = listen((*sockfd), BACKLOG);
	if (err == -1) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	sa.sa_handler = sigchld_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;

	err = sigaction(SIGCHLD, &sa, NULL);
	if (err == -1) {
		perror("sigaction");
		exit(EXIT_FAILURE);
	}

	printf("server: waiting for connections...\n");
}

char* recv_msg(int* sockfd)
{
	char* ret;
	int numbytes;

	ret = malloc(sizeof(char) * MAXDATALEN);

	numbytes = recv((*sockfd), ret, MAXDATALEN - 1, 0);
	if (numbytes == -1) {
		perror("recv");
		exit(EXIT_FAILURE);
	}
	ret[numbytes] = '\0';

	return ret;
}

