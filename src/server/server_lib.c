#include <unistd.h>

#include "clients.h"
#include "server_lib.h"

void broadcast_message(const Clients *clients_fd, const char *buffer, size_t size) {
        const Client *c;

        if (clients_fd == NULL || clients_fd->head == NULL) {
                return;
        }

        c = clients_fd->head;
        while (c != NULL) {
                write(c->fd, buffer, size);
                c = c->next;
        }
}
