#include <stddef.h>
#include <stdio.h>
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

        if (clients == NULL || clients->head == NULL) {
                return;
        }

        c = clients->head;
        while (c != NULL) {
                write(c->fd, buffer, size);
                c = c->next;
        }
}

int server_async_init(int server_fd) {
        int epollfd = async_init();
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
static void disconnect_client(Clients *clients, int fd) {
        delete_client(clients, fd);
        close(fd);
        info("%d: connection closed\n", fd);
}

void client_message_handling(Clients *clients, int fd) {
        char buffer[MAXMSG] = {0};
        ssize_t status = read(fd, buffer, sizeof(buffer));

        if (status == -1) { //error
                return;
        }

        if (status == 0) { //connection closed
                disconnect_client(clients, fd);
        } else {
                broadcast_message(clients, buffer, sizeof(buffer));
                info("%d: %.*s\n", fd, MAXMSG, buffer);
        }
}

/**
 * Add client to clients' list and store client's fd
 */
static int connect_client(Clients *clients, int fd, int epollfd) {
        int err;

        err = add_client(clients, fd);
        if (err == -1) {
                info("%d: add_client failed\n", fd);
                return -1;
        }

        err = async_add(epollfd, fd, EPOLLIN | EPOLLET);
        if (err == -1) {
                delete_client(clients, fd);
                info("%d: async_add failed\n", fd);
                return -1;
        }

        info("%d: connection opened\n", fd);
        return 0;
}

void accept_client(Clients *clients, int epollfd, int server_fd) {
        ssize_t err;
        int fd = accept4(server_fd, NULL, NULL, SOCK_NONBLOCK);
        if (fd == -1) {
                perror("accept4");
                return;
        }

        err = connect_client(clients, fd, epollfd);
        if (err == -1) {
                return;
        }

        err = write(fd, CHAT_BANNER, sizeof(CHAT_BANNER));
        if (err == -1) {
                info("%d: error sending CHAT_BANNER\n", fd);
                return;
        }
}
