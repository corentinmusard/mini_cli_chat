#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>

#include "client_lib.h"
#include "log.h"
#include "utils.h"
#include "cli.h"

int server_message_handling(WINDOW *messages_window, int sockfd, int j) {
        char buffer[MAXMSG] = {0};
        char *formated_message;
        long int status;

        status = recv(sockfd, buffer, sizeof(buffer), 0);
        if (status == -1 || status == 0) { //error or connection to server closed
                perror("recv");
                return -1;
        }

        formated_message = log_format(buffer, sizeof(buffer));
        mvwprintw(messages_window, j, 1, formated_message);

        free(formated_message);
        return 0;
}

void delete_message_character(WINDOW *input_window, char *buffer_message, int *i_message) {
        buffer_message[*i_message] = '\0';
        (*i_message)--;
        if (*i_message < 0) {
                *i_message = 0;
        }
        wmove(input_window, 1, INITIAL_MESSAGE_X + *i_message);
        wdelch(input_window);
}

void increment_indice_message(const WINDOW *input_window, int *i_message) {
        int max_x = getmaxx(input_window) - INITIAL_MESSAGE_X;

        (*i_message)++;
        if (*i_message > max_x) {
                *i_message = max_x;
        }
}

void reset_variables(char *buffer_message, size_t n, int *i_message) {
        memset(buffer_message, 0, n);
        *i_message = 0;
}
