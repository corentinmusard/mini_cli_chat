#ifndef SERVER_NETWORK_H
#define SERVER_NETWORK_H
#ifdef __cplusplus
extern "C" {
#endif

#include <netinet/in.h>

/**
 * socket() call failed
 */
#define SOCKET_FAILED (-1)

/**
 * bind() call failed
 */
#define BIND_FAILED (-2)

/**
 * listen() call failed
 */
#define LISTEN_FAILED (-3)

/**
 * Maximum length to which the queue of pending connections for server_sock_fd may grow.
 * See listen(2) for more infos
 */
#define LISTEN_BACKLOG 50

/**
 * Connect the server to a port
 * Return the socket fd
 * Return a negative value on failure
 */
int start_server(in_port_t port);

/**
 * Close socket
 */
void stop_server(int server_sock_fd);

#ifdef __cplusplus
}
#endif
#endif
