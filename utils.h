#ifndef __UTILS_H__
#define __UTILS_H__

#include <curses.h>
#include <stdlib.h>

#define handle_error(msg) \
           do { perror(msg); endwin(); exit(EXIT_FAILURE); } while (0)

// client and server
#define PORT 5000
#define MAXMSG 100
#define MAX_EVENTS 10

// client only
// none

// server only
#define LISTEN_BACKLOG 50
#define CHAT_BANNER "Welcome to the basic chat!"

#endif
