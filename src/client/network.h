#ifndef CLIENT_NETWORK_H
#define CLIENT_NETWORK_H
#ifdef __cplusplus
extern "C" {
#endif

#include <netinet/in.h>
#include <stdint.h>

/**
 * Connect the client to ip:port
 * Return the socket fd
 * Return a negative value on failure
 */
int connect_client(uint32_t ip, in_port_t port);

#ifdef __cplusplus
}
#endif
#endif
