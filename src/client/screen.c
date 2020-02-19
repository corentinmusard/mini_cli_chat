#include <stdlib.h>
#include <assert.h>

#include "cli.h"
#include "screen.h"
#include "utils.h"

Input* input_init(void) {
        Input *input = malloc(sizeof(Input));
        assert(input);

        input->window = NULL;
        input->i = 0;
        input->buffer = calloc(MAXMSG, sizeof(char));

        assert(input->buffer);

        return input;
}

void free_input(Input *input) {
        free(input->buffer);
        free(input);
}

Messages* messages_init(void) {
        Messages *m = malloc(sizeof(Messages));
        assert(m);

        m->window = NULL;
        m->y = 0;

        return m;
}

void free_messages(Messages *messages) {
        free(messages);
}

Screen *screen_init(void) {
        Screen *s = malloc(sizeof(Screen));
        assert(s);

        s->msgs = messages_init();
        s->input = input_init();

        init_cli(&s->msgs->window, &s->input->window);
        assert(s->msgs->window);
        assert(s->input->window);

        return s;
}

void free_screen(Screen *s) {
        free_messages(s->msgs);
        free_input(s->input);
        free(s);
}

void refresh_screen(Screen *s) {
        refresh_cli(s->msgs->window, s->input->window);
}
