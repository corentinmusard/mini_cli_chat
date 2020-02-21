#ifndef SERVER_CLIENTS_H
#define SERVER_CLIENTS_H
#ifdef __cplusplus
extern "C" {
#endif

typedef struct Client_s {
        struct Client_s *next; // next client or NULL
        int fd; // client' socket fd
} Client;

typedef struct {
        Client *head; // first client
        int nb; // nb of client
} Clients;

/**
 * Initialize and return a clients list
 */
Clients* init_clients(void);

/**
 * Add a client with value `fd` at the beginning of the linked list `l`
 * Fail and return 0 if `fd` is less than 0
 * Otherwise return 1
 */
int add_client(Clients *l, int fd);

/**
 * Delete the first client from `l` who has `fd`
 */
void delete_client(Clients *l, int fd);

/**
 * Free all clients
 */
void free_clients(Clients *l);

#ifdef __cplusplus
}
#endif
#endif
