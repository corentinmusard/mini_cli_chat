#ifndef SERVER_SERVER_UTILS_H
#define SERVER_SERVER_UTILS_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

#include "clients.h"

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
