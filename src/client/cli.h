#ifndef CLIENT_CLI_H
#define CLIENT_CLI_H
#ifdef __cplusplus
extern "C" {
#endif

#include <curses.h>

/**
 * Initial x coordinate for input window
 * First 3 chars are INPUT_PROMPT
 */
#define INITIAL_MESSAGE_X 4

/**
 * Initial y coordinate for input window
 */
#define INITIAL_MESSAGE_Y 0

/**
 * Initialize ncurses
 * Create and return 2 subwindow
 * messages_window is for displaying messages received from server
 * input_window is for new message typing
 */
void init_cli(WINDOW **messages_window, WINDOW **input_window);

/**
 * Refresh both windows
 */
void refresh_cli(WINDOW *messages_window, WINDOW *input_window);

/**
 * Clear `input_window`
 * Move cursor to the beginning of the window,
 * to be able to type another message
 */
void clear_message_area(WINDOW *input_window);

#ifdef __cplusplus
}
#endif
#endif
