#ifndef TESTS_FAKE_FUNCTIONS_H
#define TESTS_FAKE_FUNCTIONS_H

#include <curses.h>
#include <fff/fff.h>

//declare fake curses functions
DECLARE_FAKE_VALUE_FUNC(int, doupdate)
DECLARE_FAKE_VALUE_FUNC_VARARG(int, mvwprintw, WINDOW *, int, int, const char *, ...)
DECLARE_FAKE_VALUE_FUNC(int, wclrtoeol,WINDOW *)
DECLARE_FAKE_VALUE_FUNC(int, wdelch, WINDOW *)
DECLARE_FAKE_VALUE_FUNC(int, wmove, WINDOW *, int, int)
DECLARE_FAKE_VALUE_FUNC(int, wnoutrefresh, WINDOW *)

#endif
