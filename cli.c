#include <curses.h>

#include "cli.h"

void init_cli(WINDOW **messages_window, WINDOW **input_window) {
        initscr();
        *messages_window = subwin(stdscr, LINES-2, COLS, 0, 0);
        *input_window = subwin(stdscr, 2, COLS, LINES-2, 0);
        mvwprintw(*messages_window, 1, 1, "Ceci est la fenetre du messages_window\n");
        mvwprintw(*messages_window, 2, 1, "Salut toi\n");
        mvwprintw(*input_window, 1, 1, ">> ");
        box(*messages_window, ACS_VLINE, ACS_HLINE);
        move(LINES-1, 4);
}
