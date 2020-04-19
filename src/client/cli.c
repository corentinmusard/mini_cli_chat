#include <assert.h>
#include <curses.h>

#include "cli.h"

/**
 * The string printed before input
 */
#define INPUT_PROMPT ">> "

/**
 * The number of space to leave before printing prompt
 */
#define INPUT_PROMPT_X 1

/**
 * The number of line of the input subwindow
 */
#define NLINES_INPUT 1

/**
 * Reset message cursor juste after the ">> " chars
 */
static void reset_message_cursor(WINDOW *input_window) {
        assert(input_window && "should not be NULL");

        wmove(input_window, INITIAL_MESSAGE_Y, INITIAL_MESSAGE_X);
}

void init_cli(WINDOW **messages_window, WINDOW **input_window) {
        assert(messages_window && "should not be NULL");
        assert(input_window && "should not be NULL");

        initscr();

        *messages_window = subwin(stdscr, LINES-NLINES_INPUT, COLS, 0, 0);
        *input_window = subwin(stdscr, NLINES_INPUT, COLS, LINES-NLINES_INPUT, 0);
        assert(*messages_window);
        assert(*input_window);

        touchwin(stdscr); //necessary before refreshing subwindows

        mvwprintw(*input_window, INITIAL_MESSAGE_Y, INPUT_PROMPT_X, INPUT_PROMPT);
        box(*messages_window, ACS_VLINE, ACS_HLINE);

        reset_message_cursor(*input_window);
}

void refresh_cli(WINDOW *messages_window, WINDOW *input_window) {
        assert(messages_window && "should not be NULL");
        assert(input_window && "should not be NULL");

        wnoutrefresh(messages_window);
        wnoutrefresh(input_window);
        doupdate();
}

void clear_message_area(WINDOW *input_window) {
        assert(input_window && "should not be NULL");

        reset_message_cursor(input_window);
        wclrtoeol(input_window);
}
