#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include "asynchronous.h"
#include "clients.h"
#include "log.h"
#include "server_lib.h"
#include "utils.h"

/**
 * Message sent by server to a client after sucessful connection
 */
#define CHAT_BANNER "Welcome to the basic chat!"

/**
 * Send message `buffer` of size `size` to each client in `clients`
 */
static void broadcast_message(const Clients *clients, const char *buffer, size_t size) {
        const Client *c;

        assert(clients && "should not be NULL");
        assert(buffer && "should not be NULL");
        assert(size > 0 && "should not be 0");

        c = clients->head;
        while (c != NULL) {
                write(c->fd, buffer, size);
                c = c->next;
        }
}

int server_async_init(int server_fd) {
        int epollfd;

        assert(server_fd >= 0 && "should be a valid file descriptor");

        epollfd = async_init();
        if (epollfd == -1) {
                return -1;
        }

        if (async_add(epollfd, server_fd, EPOLLIN) == -1) {
                return -1;
        }

        return epollfd;
}

/**
 * Remove client from clients' list and close client's fd
 */
static void disconnect_client(Client *c) {
        assert(c && "should not be NULL");
        assert(c->list && "should not be NULL");
        assert(c->fd && "should not be NULL");

        info("%s: connection closed\n", c->username);
        close(c->fd);
        delete_client(c);
}

/**
 * Execute special command starting with '/'
 */
static void special_command_handling(Client *c, char *buffer) {
        char *token;

        assert(c && "should not be NULL");
        assert(buffer && "should not be NULL");

        token = strtok(buffer, " ");
        if (token == NULL) {
                return;
        }

        if (strcmp("/NICK", token) == 0) {
                char *username = strtok(NULL, " ");
                if (username == NULL) {
                        return;
                }
                // update username
                memset(c->username, 0, MAX_USERNAME_LENGTH);
                strncpy(c->username, username, MAX_USERNAME_LENGTH);
                c->username[MAX_USERNAME_LENGTH] = '\0';
        } else { //command unknown

        }
}

void client_message_handling(Client *c) {
        char msg[MAXMSG_SERV] = {0};
        char buffer[MAXMSG_CLI] = {0};
        ssize_t status;

        assert(c && "should not be NULL");

        status = read(c->fd, buffer, sizeof(buffer));
        if (status == -1) { //error
                return;
        }

        if (status == 0) { //connection closed
                snprintf(msg, MAXMSG_SERV, "%s: leave the server\n", c->username);

                broadcast_message(c->list, msg, sizeof(msg));
                info(msg);
                disconnect_client(c);
                return;
        }

        if (buffer[0] == '/') { // special command
                special_command_handling(c, buffer);
                return;
        }

        snprintf(msg, MAXMSG_SERV, "%s: %s\n", c->username, buffer);
        broadcast_message(c->list, msg, sizeof(msg));
        info(msg);
}

/**
 * Add client to clients' list and store client's fd
 * Return NULL on failure
 * Return client on success
 */
static Client* connect_client(Clients *clients, int fd, int epollfd) {
        Client *c;
        int err;

        assert(clients && "should not be NULL");
        assert(fd >= 0 && "should be a valid file descriptor");
        assert(epollfd >= 0 && "should be a valid file descriptor");

        err = async_add(epollfd, fd, EPOLLIN | EPOLLET);
        if (err == -1) {
                info("%d: async_add failed\n", fd);
                return NULL;
        }

        c = add_client(clients, fd);
        info("%s: connection opened\n", c->username);
        return c;
}

void accept_client(Clients *clients, int epollfd, int server_fd) {
        char msg[MAXMSG_SERV] = {0};
        ssize_t err;
        Client *c;
        int fd;

        assert(clients && "should not be NULL");
        assert(epollfd >= 0 && "should be a valid file descriptor");
        assert(server_fd >= 0 && "should be a valid file descriptor");

        fd = accept4(server_fd, NULL, NULL, SOCK_NONBLOCK);
        if (fd == -1) {
                perror("accept4");
                return;
        }

        c = connect_client(clients, fd, epollfd);
        if (c == NULL) {
                return;
        }

        err = write(c->fd, CHAT_BANNER, sizeof(CHAT_BANNER));
        if (err == -1) {
                info("%s: error sending CHAT_BANNER\n", c->username);
                return;
        }

        snprintf(msg, MAXMSG_SERV, "%s: join the server\n", c->username);
        broadcast_message(c->list, msg, sizeof(msg));
        info(msg);
}
