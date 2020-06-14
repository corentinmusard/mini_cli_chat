#ifndef SERVER_CLIENTS_H
#define SERVER_CLIENTS_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

/**
 * Max username length
 */
#define MAX_USERNAME_LENGTH 20

typedef struct Client_s Client;
typedef struct Clients_s Clients;

/**
 * A client
 */
struct Client_s {
    Clients *list; // list of client containing this client
    Client *next; // next client or NULL

    char username[MAX_USERNAME_LENGTH + 1]; // client's username
    int fd; // client' socket fd
};

/**
 * A list of client
 */
struct Clients_s {
    Client *head; // first client
    int nb; // number of client
};

/**
 * Initialize and return a clients list
 */
Clients* init_clients(void);

/**
 * Add a client with value `fd` at the beginning of the linked list `l`
 * Return this client
 */
Client* add_client(Clients *l, int fd);

/**
 * Delete client `c` from its list
 * c is freed, don't use it afterwards
 */
void delete_client(const Client *c);

/**
 * Delete the first client from `l` who has `fd`
 * `fd` should be in `l`
 */
void delete_client_fd(Clients *l, int fd);

/**
 * Return the first client from `l` who has `fd`
 * `fd` should be in `l`
 */
Client* get_client(const Clients *l, int fd);

/**
 * Free all clients
 */
void free_clients(Clients *l);

/**
 * Return true if username isn't used by any clients
 * Return false otherwise
 */
bool is_available_username(const Clients *l, const char *username);

#ifdef __cplusplus
}
#endif
#endif
