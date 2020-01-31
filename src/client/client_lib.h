#ifndef CLIENT_LIB_H
#define CLIENT_LIB_H
#ifdef __cplusplus
extern "C" {
#endif

#include <curses.h>
#include <signal.h>

/**
 * Default value is 0
 * If value is not 0 then client is going to exit
 */
extern volatile sig_atomic_t exit_wanted;

typedef struct {
        WINDOW *window; // to type new message
        int i; // cursor indice
        char *buffer; // message
} Input;

/**
 * Initialize and return an Input
 */
Input* input_init(void);

/**
 * Free input
 */
void free_input(Input *input);

typedef struct {
        WINDOW *window; // to display messages received from server
        int y; // last message line
} Messages;

/**
 * Initialize and return a Messages
 */
Messages* messages_init(void);

/**
 * Free messages
 */
void free_messages(Messages *messages);

/**
 * Print recv message from `sockfd` and print it into messages window
 * Return -1 on failure and 0 on succes
 */
int server_message_handling(Messages *msgs, int sockfd);

/**
 * Called when SIGINT is raised
 */
void int_handler(int sig __attribute__ ((unused)));

int stdin_char_handling(Input *input, Messages *msgs, int sockfd);

#ifdef __cplusplus
}
#endif
#endif
