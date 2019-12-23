#include <stdlib.h>

#include "clients.h"

clients* init_clients(void) {
        clients *c;

        c = malloc(sizeof(clients));
        c->head = NULL;
        c->nb = 0;

        return c;
}

void add_client(clients *l, int fd) {
        client *c = malloc(sizeof(client));
        c->fd = fd;
        c->next = l->head;

        l->head = c;
        l->nb++;
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

        while (c->next != NULL) {
                if (c->next->fd == fd) {
                        client *next = c->next->next;
                        free(c->next);
                        c->next = next;
                        l->nb--;
                        break;
                }
                c = c->next;
        }
}
