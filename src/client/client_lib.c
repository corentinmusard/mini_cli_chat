#include <assert.h>
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>

#include "asynchronous.h"
#include "cli.h"
#include "client_lib.h"
#include "log.h"
#include "screen.h"
#include "utils.h"

/**
 * ASCII char 0x7f = 127
 */
#define DEL 127

/**
 * Print `message` to messages window
 */
static void print_message(Messages *msgs, const char *message) {
        msgs->y++;
        mvwprintw(msgs->window, msgs->y, 1, message);
}

/**
 * Remove char at indice `i` from `input_window` and `buffer`
 * Move cursor at indice `i-1`
 * Update value of `i`
 */
static void delete_message_character(Input *input) {
        input->buffer[input->i] = '\0';
        input->i--;
        if (input->i < 0) {
                input->i = 0;
        }
        wmove(input->window, INITIAL_MESSAGE_Y, INITIAL_MESSAGE_X + input->i);
        wdelch(input->window);
}

/**
 * Increment `i` if not superior of the size of the windows
 */
static void increment_indice_message(Input *input) {
        int y __attribute__ ((unused));
        int x;
        int max_x;

        getmaxyx(input->window, y, x);
        assert(x >= 0);

        max_x = x - INITIAL_MESSAGE_X;

        input->i++;
        if (input->i > max_x) {
                input->i = max_x;
        }
}

/**
 * Reset `input` to default value
 */
static void reset_variables(Input *input) {
        assert(input->i > 0);

        memset(input->buffer, 0, (size_t)input->i);
        input->i = 0;
}

/**
 * Execute `command`
 * Return 1 on success
 * Return 0 if `command` is unknown
 */
static int execute_command(const char *command) {
        if (strcmp("/quit", command) == 0 || strcmp("/q", command) == 0) {
                interrupt = 1;
                return 1;
        }
        return 0;
}

/**
 * Print char `c` to input window
 */
static void print_input_char(const Input *input, char c) {
        mvwprintw(input->window,
                INITIAL_MESSAGE_Y,
                INITIAL_MESSAGE_X + input->i,
                "%c", c);
}

/**
 * Store `c` in `buffer`
 * Print `c` to input window
 */
static void input_char_handling(Input *input, char c) {
        input->buffer[input->i] = c;
        print_input_char(input, c);
        increment_indice_message(input);
}

int server_message_handling(Messages *msgs, int sockfd) {
        char buffer[MAXMSG] = {0};
        char *formated_message;
        ssize_t status;

        status = read(sockfd, buffer, sizeof(buffer));
        if (status == -1 || status == 0) {  // error or connection to server closed
                perror("read");
                return -1;
        }

        formated_message = log_format(buffer, sizeof(buffer));
        print_message(msgs, formated_message);

        free(formated_message);
        return 0;
}

int stdin_char_handling(const Screen *screen, int sockfd) {
        char c;
        if (read(STDIN_FILENO, &c, 1) == -1) {
                perror("read");
                return -1;
        }

        if (c == '\r') {  // end of the message, send it
                if (screen->input->i == 0) {  // blank message
                        // don't send it
                        return 1;
                }
                if (screen->input->buffer[0] == '/') {  // start with '/'
                        // It's a command
                        if (execute_command(screen->input->buffer) == 0) {
                                // command unknown
                                print_message(screen->msgs, "Command unknown");
                        }
                        return 1;
                }
                if (write(sockfd, screen->input->buffer, (size_t)screen->input->i) == -1) {
                        perror("write");
                        return -1;
                }
                reset_variables(screen->input);
                clear_message_area(screen->input->window);
                return 1;
        }

        if (c == DEL) {
                delete_message_character(screen->input);
                return 1;
        }

        if (screen->input->i == (MAXMSG - 1)) {  // max message length reached
                // ignore character for now
                return 1;
        }

        input_char_handling(screen->input, c);
        return 1;
}

int client_async_init(int sockfd) {
        int epollfd = async_init();
        if (epollfd == -1) {
                return -1;
        }

        if (async_add(epollfd, sockfd, EPOLLIN) == -1) {
                return -1;
        }
        if (async_add(epollfd, STDIN_FILENO, EPOLLIN) == -1) {
                return -1;
        }

        return epollfd;
}
