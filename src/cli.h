#ifndef CLI_H
#define CLI_H

#include <curses.h>

/**
 * Initial x coordonate for input window
 * First 3 chars are ">> "
 */
#define INITIAL_MESSAGE_X 4

void init_cli(WINDOW **messages_window, WINDOW **input_window);

void refresh_cli(WINDOW *messages_window, WINDOW *input_window);

void clear_message_area(WINDOW *input_window);

#endif
