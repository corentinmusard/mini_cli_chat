#include <assert.h>
#include <stdlib.h>

#include "clients.h"

Clients* init_clients(void) {
        Clients *c;

        c = malloc(sizeof(Clients));
        assert(c);

        c->head = NULL;
        c->nb = 0;

        return c;
}

int add_client(Clients *l, int fd) {
        Client *c;

        if (fd < 0) {
                return -1;
        }

        c = malloc(sizeof(Client));
        c->fd = fd;
        c->next = l->head;

        l->head = c;
        l->nb++;
        return 0;
}

void delete_client(Clients *l, int fd) {
        Client *c;

        if (l == NULL || l->head == NULL) {
                return;
        }

        c = l->head;
        if (c->fd == fd) {
                l->head = c->next;
                l->nb--;
                free(c);
                return;
        }

        while (c->next != NULL && c->next->fd != fd) {
                c = c->next;
        }

        if (c->next != NULL) {
                Client *next = c->next->next;
                free(c->next);
                c->next = next;
                l->nb--;
        }
}

void free_clients(Clients *l) {
        Client *c = l->head;

        while (c != NULL) {
                Client *next = c->next;
                free(c);
                c = next;
        }

        free(l);
}
