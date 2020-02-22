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
void broadcast_message(const Clients *clients, const char *buffer, size_t size);

/**
 * Initialize async. Return fd of the epoll instance.
 * Return -1 on failure
 */
int server_async_init(int server_sock_fd);

int client_message_handling(Clients *clients, int fd);

int accept_client(Clients *clients, int epollfd, int conn_sock);

#ifdef __cplusplus
}
#endif
#endif
