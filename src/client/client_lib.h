#ifndef CLIENT_LIB_H
#define CLIENT_LIB_H

#include <curses.h>
#include <stddef.h>
#include <signal.h>

#include "../utils/utils.h"
/**
 * Default value is 0
 * If value is not 0 then client is going to exit 
 */
extern volatile sig_atomic_t exit_wanted;

/**
 * Print recv message from `sockfd` and print it into `messages_window` on line `j`
 * Return -1 on failure and 0 on succes
 */
int server_message_handling(WINDOW *messages_window, int sockfd, int j);

/**
 * Remove char at indice `i` from `input_window` and `buffer_message`
 * Move cursor at indice `i-1`
 * Update value of `i`
 */
void delete_message_character(WINDOW *input_window, char *buffer_message, int *i_message);

/**
 * Increment `i_message` if not superior of the size of the windows
 */
void increment_indice_message(const WINDOW *input_window, int *i_message);

/**
 * Reset variables `buffer_message` and `i_message` to default value
 */
void reset_variables(char *buffer_message, size_t n, int *i_message);

/**
 * Called when SIGINT is raised
 */
void int_handler(int sig __attribute__ ((unused)));

#endif
