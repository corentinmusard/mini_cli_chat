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

void add_client(Clients *l, int fd) {
        Client *c;

        assert(l && "should not be NULL");
        assert(fd >= 0 && "should be a valid file descriptor");

        c = malloc(sizeof(Client));
        assert(c);
        c->fd = fd;
        c->next = l->head;

        l->head = c;
        l->nb++;
}

void delete_client(Clients *l, int fd) {
        Client *c;

        assert(l && "should not be NULL");
        assert(l->head && "should not be NULL");
        assert(fd >= 0 && "should be a valid file descriptor");

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
        } else {
                assert(0 && "fd should be in l");
        }
}

void free_clients(Clients *l) {
        Client *c;

        assert(l && "should not be NULL");

        c = l->head;
        while (c != NULL) {
                Client *next = c->next;
                free(c);
                c = next;
        }

        free(l);
}
