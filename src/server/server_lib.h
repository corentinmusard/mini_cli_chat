#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

#include "clients.h"

/**
 * Maximum length to which the queue of pending connections for server_sock_fd may grow.
 * See man listen(2) for more infos
 */
#define LISTEN_BACKLOG 50

/**
 * Message sent by server to a client after sucessful connection
 */
#define CHAT_BANNER "Welcome to the basic chat!"

/**
 * Send message `buffer` of size `size` to each client in `clients_fd`
 */
void broadcast_message(const Clients *clients_fd, const char *buffer, size_t size);

#ifdef __cplusplus
}
#endif
#endif
