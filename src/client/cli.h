#ifndef CLI_H
#define CLI_H

#include <curses.h>

/**
 * Initial x coordinate for input window
 * First 3 chars are ">> "
 */
#define INITIAL_MESSAGE_X 4

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

#endif
