#ifndef CLIENTS_H
#define CLIENTS_H

typedef struct client_s {
        struct client_s *next;
        int fd;
} client;

typedef struct {
        client *head;
        int nb; // nb of client
} clients;

/**
 * Initialize and return a clients list
 */
clients* init_clients(void);

/**
 * Add a client with value `fd` at the beginning of the linked list `l`
 */
void add_client(clients *l, int fd);

/**
 * Delete the first client from `l` who has `fd`
 */
void delete_client(clients *l, int fd);

#endif
