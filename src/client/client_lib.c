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
    assert(msgs && "should not be NULL");
    assert(message && "should not be NULL");

    msgs->y++;
    mvwprintw(msgs->window, msgs->y, 1, message);
}

/**
 * Remove char at indice `i` from `input_window` and `buffer`
 * Move cursor at indice `i-1`
 * Update value of `i`
 */
static void delete_message_character(Input *input) {
    assert(input && "should not be NULL");

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
    assert(input && "should not be NULL");

    int y __attribute__ ((unused));
    int x;
    getmaxyx(input->window, y, x);
    assert(x >= 0);

    int max_x = x - INITIAL_MESSAGE_X;

    input->i++;
    if (input->i > max_x) {
        input->i = max_x;
    }
}

/**
 * Reset `input` to default value
 */
static void reset_variables(Input *input) {
    assert(input && "should not be NULL");
    assert(input->i > 0 && "should not be 0");

    memset(input->buffer, 0, (size_t)input->i);
    input->i = 0;
}

/**
 * Execute `command`
 * Return 0 on success
 * Return -1 if `command` is unknown
 */
static int execute_command(const char *command) {
    assert(command && "should not be NULL");

    if (strcmp("/quit", command) == 0 || strcmp("/q", command) == 0) {
        interrupt = 1;
        return 0;
    }
    return -1;
}

/**
 * Print char `c` to input window
 */
static void print_input_char(const Input *input, char c) {
    assert(input && "should not be NULL");

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
    assert(input && "should not be NULL");

    input->buffer[input->i] = c;
    print_input_char(input, c);
    increment_indice_message(input);
}

/**
 * Append date to `buffer`, display it on the screen
 */
static void display_message(Messages *msgs, const char *buffer, size_t size) {
    assert(msgs && "should not be NULL");
    assert(buffer && "should not be NULL");
    assert(size > 0 && "should not be 0");

    char *formated_message = log_format(buffer, size);
    print_message(msgs, formated_message);

    free(formated_message);
}

int server_message_handling(Messages *msgs, int sockfd) {
    assert(msgs && "should not be NULL");
    assert(sockfd >= 0 && "should be a valid file descriptor");

    char buffer[MAXMSG_SERV] = {0};
    ssize_t status = read(sockfd, buffer, sizeof(buffer));
    if (status == -1 || status == 0) {  // error or connection to server closed
        perror("read");
        return -1;
    }

    char msg[MAXMSG_SERV] = {0};
    int msg_len = 0;
    for (int i = 0; i < status; i++) {
        if (buffer[i] != '\0') {
            msg[msg_len] = buffer[i];
            msg_len++;
        } else { // end of string
            if (msg_len == 0) {  // no more message
                return 0;
            }
            display_message(msgs, msg, sizeof(msg));
            // reset variables
            memset(msg, 0, sizeof(msg));
            msg_len = 0;
        }
    }

    return 0;
}

/**
 * If message is blank, do nothing
 * If message start with '/', execute command
 * Else send message to server
 */
static int evaluate_complete_message(const Screen *s, int sockfd) {
    assert(s && "should not be NULL");
    assert(sockfd >= 0 && "should be a valid file descriptor");

    if (s->input->i == 0) {  // blank message
        // don't send it
        return 0;
    }
    if (s->input->buffer[0] == '/') {  // start with '/'
        // It's a command
        if (execute_command(s->input->buffer) == -1) {
            // command unknown
            print_message(s->msgs, "Command unknown");
        }
        return 0;
    }
    if (write(sockfd, s->input->buffer, (size_t)s->input->i) == -1) {
        perror("write");
        return -1;
    }
    reset_variables(s->input);
    clear_message_area(s->input->window);
    return 0;
}

int stdin_char_handling(const Screen *s, int sockfd) {
    assert(s && "should not be NULL");
    assert(sockfd >= 0 && "should be a valid file descriptor");

    char c;
    if (read(STDIN_FILENO, &c, 1) == -1) {
        perror("read");
        return -1;
    }

    switch (c) {
        case '\r':
            return evaluate_complete_message(s, sockfd);
        case '\t':
            break; // ignore tab for now
        case DEL:
            delete_message_character(s->input);
            break;
        default:
            if (s->input->i == (MAXMSG_CLI - 1)) {  // max message length reached
                break; // ignore character for now
            }
            input_char_handling(s->input, c);
    }
    return 0;
}

int client_async_init(int sockfd) {
    assert(sockfd >= 0 && "should be a valid file descriptor");

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
