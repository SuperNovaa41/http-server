#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#include "file.h"
#include "http.h"
#include "tcp.h"

int main(void)
{
	int sockfd, new_fd; 
	struct sockaddr_storage their_addr; 
	socklen_t sin_size;

	int err;

	char connection_address[INET6_ADDRSTRLEN];

	setup_tcp_server(&sockfd);

	char* body = read_file("../html/index.html");
	char* response = generate_http_message(HTTP_OK, "text/html", body);
	free(body);

	while (1) { // main accept loop
		sin_size = sizeof(their_addr);
		new_fd = accept(sockfd, (struct sockaddr*) &their_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");
			continue;
		}

		inet_ntop(their_addr.ss_family, 
				get_in_addr((struct sockaddr*) &their_addr),
				connection_address, sizeof(connection_address));
		printf("server: got connection from %s\n", connection_address);

		if (!fork()) { // this is the child process
			close(sockfd); // child doesn't need the listener
			
			
			
			err = send(new_fd, response, strlen(response), 0);
			if (err == -1)
				perror("send");

			printf("Lost connection from: %s\n", connection_address);
			close(new_fd);
			exit(0);
		}

		close(new_fd);
	}

	free(response);

	return 0;
}
