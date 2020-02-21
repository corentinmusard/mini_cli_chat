#ifndef CLIENT_CLIENT_LIB_H
#define CLIENT_CLIENT_LIB_H
#ifdef __cplusplus
extern "C" {
#endif

#include "screen.h"

/**
 * Print recv message from `sockfd` and print it into messages window
 * Return -1 on failure and 0 on succes
 */
int server_message_handling(Messages *msgs, int sockfd);

int stdin_char_handling(const Screen *screen, int sockfd);

int client_async_init(int sockfd);

#ifdef __cplusplus
}
#endif
#endif
