#include <asm-generic/socket.h>
#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <signal.h>

#define PORT "3490" 

#define BACKLOG 10 // the amount of pending connections the queue will hold

void sigchld_handler(int s)
{
	//waitpid() might overwrite errno, so we save and restore it:
	int saved_errno = errno;

	while (waitpid(-1, NULL, WNOHANG) > 0)
		;

	errno = saved_errno;
}

// get sockaddr, IPv4 or IPv6

void* get_in_addr(struct sockaddr* sa)
{
	if (sa->sa_family == AF_INET)
		return &(((struct sockaddr_in*)sa)->sin_addr);
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
	int sockfd, new_fd; // listen on sockfd, new connection on new_fd
	
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address info
	
	socklen_t sin_size;

	struct sigaction sa;

	char s[INET6_ADDRSTRLEN];

	char test_response[] = "HTTP/1.1 200 OK\nDate: Sat, 10 Feb 2024 16:15:55 EST\nETag: \"123-a\"\nContent-Length: 12\nVary: Accept-Encoding\nContent-Type: text/plain\n\nHello World!\n";

	int yes = 1;
	int err;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use MY ip
	
	err = getaddrinfo(NULL, PORT, &hints, &servinfo);

	if (err != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
		exit(EXIT_FAILURE);
	}

	for (p = servinfo; p != NULL; p = p->ai_next) {
		sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (sockfd == -1) {
			perror("server: socket");
			continue;
		}

		err = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
		if (err == -1) {
			perror("setsockopt");
			exit(EXIT_FAILURE);
		}

		err = bind(sockfd, p->ai_addr, p->ai_addrlen);
		if (err == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}

		break;
	}

	freeaddrinfo(servinfo); // all done with this structure


	if (p == NULL) {
		fprintf(stderr, "server: failed to bind\n");
		exit(EXIT_FAILURE);
	}

	err = listen(sockfd, BACKLOG);
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

	while (1) { // main accept loop
		sin_size = sizeof(their_addr);
		new_fd = accept(sockfd, (struct sockaddr*) &their_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");
			continue;
		}

		inet_ntop(their_addr.ss_family, 
				get_in_addr((struct sockaddr*) &their_addr),
				s, sizeof(s));
		printf("server: got connection from %s\n", s);

		if (!fork()) { // this is the child process
			close(sockfd); // child doesn't need the listener
			
			err = send(new_fd, test_response, strlen(test_response), 0);
			if (err == -1)
				perror("send");

			printf("Lost connection from: %s\n", s);
			close(new_fd);
			exit(0);
		}

		close(new_fd);
	}

	return 0;
}
