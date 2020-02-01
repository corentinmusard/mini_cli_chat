#include <stdlib.h>
#include <assert.h>

#include "clients.h"

clients* init_clients(void) {
        clients *c;

        c = malloc(sizeof(clients));
        assert(c);

        c->head = NULL;
        c->nb = 0;

        return c;
}

int add_client(clients *l, int fd) {
        client *c;

        if (fd < 0) {
                return 0;
        }

        c = malloc(sizeof(client));
        c->fd = fd;
        c->next = l->head;

        l->head = c;
        l->nb++;
        return 1;
}

void delete_client(clients *l, int fd) {
        client *c;

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

        if (c->next != NULL && c->next->fd == fd) {
                client *next = c->next->next;
                free(c->next);
                c->next = next;
                l->nb--;
        }
}

void free_clients(clients *l) {
        client *c;

        if (l == NULL) {
                return;
        }

        c = l->head;
        while (c != NULL) {
                client *next = c->next;
                free(c);
                c = next;
        }

        free(l);
}
