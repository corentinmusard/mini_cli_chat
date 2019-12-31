#ifndef CLIENT_LIB_H
#define CLIENT_LIB_H

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
 * Print `message` to messages window
 */
void print_message(Messages *msgs, const char *message);

/**
 * Remove char at indice `i` from `input_window` and `buffer`
 * Move cursor at indice `i-1`
 * Update value of `i`
 */
void delete_message_character(Input *input);

/**
 * Increment `i` if not superior of the size of the windows
 */
void increment_indice_message(Input *input);

/**
 * Reset `input` to default value
 */
void reset_variables(Input *input);

/**
 * Called when SIGINT is raised
 */
void int_handler(int sig __attribute__ ((unused)));

/**
 * Execute `command`
 * Return 1 on success
 * Return 0 if `command` is unknown
 */
int execute_command(const char *command);

/**
 * Print char `c` to input window
 */
void print_input_char(Input *input, char c);

/**
 * Store `c` in `buffer`
 * Print `c` to input window
 */
void input_char_handling(Input *input, char c);

#endif
