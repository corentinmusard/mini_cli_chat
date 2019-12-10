#ifndef CLI_H
#define CLI_H

#include <curses.h>

void init_cli(WINDOW **messages_window, WINDOW **input_window);

void refresh_cli(WINDOW *messages_window, WINDOW *input_window);

void clear_message_area(WINDOW *input_window);

#endif
