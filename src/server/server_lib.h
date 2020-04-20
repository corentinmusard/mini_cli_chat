#ifndef SERVER_SERVER_UTILS_H
#define SERVER_SERVER_UTILS_H
#ifdef __cplusplus
extern "C" {
#endif

#include "clients.h"

/**
 * Initialize async. Return fd of the epoll instance.
 * Return -1 on failure
 */
int server_async_init(int server_fd);

void client_message_handling(Client *c);

void accept_client(Clients *clients, int epollfd, int server_fd);

#ifdef __cplusplus
}
#endif
#endif
