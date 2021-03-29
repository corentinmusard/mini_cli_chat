#ifndef CLIENT_SCREEN_H
#define CLIENT_SCREEN_H
#ifdef __cplusplus
extern "C" {
#endif

#include <curses.h>

#include "utils/utils.h"

typedef struct {
    WINDOW *window; // to type new message
    int i; // cursor indice
    char *buffer; // message
} Input;

/**
 * Initialize and return an Input
 */
Input* input_init(void);

/**
 * Free input
 */
void free_input(Input *input);

typedef struct Message_line_s Message_line;

struct Message_line_s {
    Message_line *previous; // previous message or NULL
    Message_line *next; // next message or NULL
    char msg[MAXMSG_SERV + 1]; // the message
};
//solution: array à taille fixe
typedef struct {
    WINDOW *window; // to display messages received from server
    int y; // last message line
    Message_line *last_msg; // last message visible on the windows
    Message_line *first_msg; // first message visible on the windows
} Messages;

/**
 * Initialize and return a Messages
 */
Messages* messages_init(void);

/**
 * Free messages
 */
void free_messages(Messages *messages);

/**
 *
 */
void store_message(Messages *messages, const char *msg);

typedef struct {
    Messages *msgs;
    Input *input;
} Screen;

/**
 * Initialize and return a Screen
 */
Screen *screen_init(void);

/**
 * Free Screen
 */
void free_screen(Screen *s);

/**
 * Refresh the entire screen i.e. both subwindows
 */
void refresh_screen(const Screen *s);

#ifdef __cplusplus
}
#endif
#endif
