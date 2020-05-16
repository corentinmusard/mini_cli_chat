#include "fake_curses.hpp"

//needed by FFF, it's a macro
DEFINE_FFF_GLOBALS

//define fake functions
DEFINE_FAKE_VALUE_FUNC(int, doupdate)
DEFINE_FAKE_VALUE_FUNC_VARARG(int, mvwprintw, WINDOW *, int, int, const char *, ...)
DEFINE_FAKE_VALUE_FUNC(int, wclrtoeol,WINDOW *)
DEFINE_FAKE_VALUE_FUNC(int, wdelch, WINDOW *)
DEFINE_FAKE_VALUE_FUNC(int, wmove, WINDOW *, int, int)
DEFINE_FAKE_VALUE_FUNC(int, wnoutrefresh, WINDOW *)
