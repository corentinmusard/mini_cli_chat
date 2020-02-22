#ifndef CLIENT_CLIENT_NETWORK_H
#define CLIENT_CLIENT_NETWORK_H
#ifdef __cplusplus
extern "C" {
#endif

#include <netinet/in.h>

/**
 * socket() call failed
 */
#define SOCKET_FAILED (-1)

/**
 * connect() call failed
 */
#define CONNECT_FAILED (-2)

/**
 * Connect the client to ip:port
 * Return the socket fd
 * Return a negative value on failure
 */
int connect_client(const char *ip, in_port_t port);

/**
 * Close socket
 */
void disconnect_client(int sockfd);

#ifdef __cplusplus
}
#endif
#endif
