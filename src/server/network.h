#ifndef CLIENT_NETWORK_H
#define CLIENT_NETWORK_H
#ifdef __cplusplus
extern "C" {
#endif

#include <netinet/in.h>

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
int connect_server(in_port_t port);

#ifdef __cplusplus
}
#endif
#endif
