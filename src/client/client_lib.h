#ifndef CLIENT_LIB_H
#define CLIENT_LIB_H
#ifdef __cplusplus
extern "C" {
#endif

#include <signal.h>

#include "screen.h"

/**
 * Default value is 0
 * If value is not 0 then client is going to exit
 */
extern volatile sig_atomic_t exit_wanted;

/**
 * Print recv message from `sockfd` and print it into messages window
 * Return -1 on failure and 0 on succes
 */
int server_message_handling(Messages *msgs, int sockfd);

/**
 * Called when SIGINT is raised
 */
void int_handler(int sig __attribute__ ((unused)));

int stdin_char_handling(Screen *screen, int sockfd);

#ifdef __cplusplus
}
#endif
#endif
