#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <unistd.h>
#include <signal.h>

#include "client_lib.h"
#include "cli.h"
#include "../utils/log.h"
#include "../utils/utils.h"

volatile sig_atomic_t exit_wanted = 0;

int server_message_handling(WINDOW *messages_window, int sockfd, int j) {
        char buffer[MAXMSG] = {0};
        char *formated_message;
        ssize_t status;

        status = read(sockfd, buffer, sizeof(buffer));
        if (status == -1 || status == 0) { //error or connection to server closed
                perror("read");
                return -1;
        }

        formated_message = log_format(buffer, sizeof(buffer));
        print_message(messages_window, j, formated_message);

        free(formated_message);
        return 0;
}

void print_message(WINDOW *messages_window, int y, const char *message) {
        mvwprintw(messages_window, y, 1, message);
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

void int_handler(int sig __attribute__ ((unused))) {
        exit_wanted = 1;
}

int execute_command(const char *command) {
        if (strcmp("/quit", command) == 0 || strcmp("/q", command) == 0) {
                exit_wanted = 1;
                return 1;
        }
        return 0;
}
