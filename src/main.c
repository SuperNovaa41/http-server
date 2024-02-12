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
	
	char *body, *response, *request;
	char *actualfilepath;
	const char* filepath;

	int err;

	char connection_address[INET6_ADDRSTRLEN];

	setup_tcp_server(&sockfd);

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

		request = recv_msg(&new_fd);
		filepath = parse_http_request(request);
		puts(filepath);

		asprintf(&actualfilepath, "../html%s/index.html", filepath);

		body = read_file(actualfilepath);
		response = generate_http_message(HTTP_OK, "text/html", body);
	
		free(body);
		free(request);


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

		free(response);
	}


	return 0;
}
