#ifndef CLIENT_LIB_H
#define CLIENT_LIB_H

#include <curses.h>
#include <stddef.h>

#include "utils.h"

int server_message_handling(WINDOW *messages_window, int sockfd, int j);

void delete_message_character(WINDOW *input_window, char *buffer_message, int *i_message);

void update_indice_message(const WINDOW *input_window, int *i_message);

void reset_variables(char buffer_message[MAXMSG], size_t n, int *i_message);

#endif