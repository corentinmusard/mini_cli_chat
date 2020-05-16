#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clients.h"

Clients* init_clients(void) {
    Clients *c = malloc(sizeof(Clients));
    assert(c);

    c->head = NULL;
    c->nb = 0;

    return c;
}

/**
 * Initialize and return a client
 */
static Client* init_client(Clients *l, int fd) {
    assert(l && "should not be NULL");
    assert(fd >= 0 && "should be a valid file descriptor");

    Client *c = malloc(sizeof(Client));
    assert(c);

    c->list = l;
    c->next = NULL;

    char username[MAX_USERNAME_LENGTH] = {0};
    int i = 0;
    do {
        memset(username, 0, MAX_USERNAME_LENGTH);
        snprintf(username, MAX_USERNAME_LENGTH, "user%d", fd+i);
        i++;
    } while (!is_available_username(l, username));
    strncpy(c->username, username, MAX_USERNAME_LENGTH);

    c->fd = fd;

    return c;
}

Client* add_client(Clients *l, int fd) {
    assert(l && "should not be NULL");
    assert(fd >= 0 && "should be a valid file descriptor");

    Client *c = init_client(l, fd);
    c->next = l->head;

    l->head = c;
    l->nb++;

    return c;
}

void delete_client(const Client *c) {
    assert(c && "should not be NULL");

    delete_client_fd(c->list, c->fd);
}

void delete_client_fd(Clients *l, int fd) {
    assert(l && "should not be NULL");
    assert(l->head && "should not be NULL");
    assert(fd >= 0 && "should be a valid file descriptor");

    Client *c = l->head;
    if (c->fd == fd) {
        l->head = c->next;
        l->nb--;
        free(c);
        return;
    }

    assert(c->next != NULL && "fd should be in l");
    while (c->next->fd != fd) {
        c = c->next;
        assert(c->next != NULL && "fd should be in l");
    }

    Client *next = c->next->next;
    free(c->next);
    c->next = next;
    l->nb--;
}

Client* get_client(const Clients *l, int fd) {
    assert(l && "should not be NULL");
    assert(l->head && "should not be NULL");
    assert(fd >= 0 && "should be a valid file descriptor");

    Client *c = l->head;
    if (c->fd == fd) {
        return c;
    }

    assert(c->next != NULL && "fd should be in l");
    while (c->next->fd != fd) {
        c = c->next;
        assert(c->next != NULL && "fd should be in l");
    }

    return c->next;
}

void free_clients(Clients *l) {
    assert(l && "should not be NULL");

    Client *c = l->head;
    while (c != NULL) {
        Client *next = c->next;
        free(c);
        c = next;
    }

    free(l);
}

bool is_available_username(const Clients *l, const char *username) {
    assert(l && "should not be NULL");
    assert(username && "should not be NULL");

    if (l->head == NULL) {
        return true;
    }

    const Client *c = l->head;
    if (strcmp(c->username, username) == 0) {
        return false;
    }

    while (c->next != NULL && strcmp(c->username, username) != 0) {
        c = c->next;
    }

    if (c->next != NULL) {
        return false;
    }

    return true;
}
