#ifndef TCP_H
#define TCP_H

#define PORT "3490" 

#define BACKLOG 10 // the amount of pending connections the queue will hold

void sigchld_handler(int s);

// get sockaddr, IPv4 or IPv6
void* get_in_addr(struct sockaddr* sa);

/**
 * void initialize_servarrdinfo
 *
 * struct addrinfo* servinfo - The servers address info struct
 *
 * Sets up the server info struct
 */
void initialize_servaddrinfo(struct addrinfo** servinfo);

/**
 * void setup_socket
 *
 * struct addrinfo* servinfo - The servers address info struct
 * int* sockfd - The socket
 *
 * Sets up the socket and binds it
 */
void setup_socket(struct addrinfo* servinfo, int* sockfd);

/**
 * void setup_tcp_server
 *
 * int* sockfd - The socket
 *
 * Sets up the tcp socket server
 */
void setup_tcp_server(int* sockfd);

#endif
