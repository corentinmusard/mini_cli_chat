#include <curses.h>

#include "cli.h"

/**
 * Reset message cursor juste after the ">> " chars
 */
static void reset_message_cursor(WINDOW *input_window) {
        wmove(input_window, 1, INITIAL_MESSAGE_X);
}

void init_cli(WINDOW **messages_window, WINDOW **input_window) {
        initscr();
        *messages_window = subwin(stdscr, LINES-2, COLS, 0, 0);
        *input_window = subwin(stdscr, 2, COLS, LINES-2, 0);
        mvwprintw(*messages_window, 1, 1, "Ceci est la fenetre du messages_window\n");
        mvwprintw(*messages_window, 2, 1, "Salut toi\n");
        mvwprintw(*input_window, 1, 1, ">> ");
        box(*messages_window, ACS_VLINE, ACS_HLINE);

        reset_message_cursor(*input_window);
}

void refresh_cli(WINDOW *messages_window, WINDOW *input_window) {
        wrefresh(messages_window);
        wrefresh(input_window);
}

void clear_message_area(WINDOW *input_window) {
        reset_message_cursor(input_window);
        wclrtoeol(input_window);
}
