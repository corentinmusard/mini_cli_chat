#ifndef SCREEN_H
#define SCREEN_H
#ifdef __cplusplus
extern "C" {
#endif

#include <curses.h>

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

typedef struct {
        WINDOW *window; // to display messages received from server
        int y; // last message line
} Messages;

/**
 * Initialize and return a Messages
 */
Messages* messages_init(void);

/**
 * Free messages
 */
void free_messages(Messages *messages);


#ifdef __cplusplus
}
#endif
#endif
