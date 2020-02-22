#include <stddef.h>
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
 * Send message `buffer` of size `size` to each client in `clients_fd`
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

int server_async_init(int server_sock_fd) {
        int epollfd = async_init();
        if (epollfd == -1) {
                return -1;
        }

        if (async_add(epollfd, server_sock_fd, EPOLLIN) == -1) {
                return -1;
        }

        return epollfd;
}

int client_message_handling(Clients *clients, int fd) {
        char buffer[MAXMSG] = {0};
        ssize_t status = read(fd, buffer, sizeof(buffer));

        if (status == -1) { //error
                return -1;
        }

        if (status == 0) { //connection closed
                info("Connection closed: %d\n", fd);
                close(fd);
                delete_client(clients, fd);
        } else {
                broadcast_message(clients, buffer, sizeof(buffer));
                info("%.*s\n", MAXMSG, buffer);
        }
        return 0;
}

int accept_client(Clients *clients, int epollfd, int server_sock_fd) {
        ssize_t err;
        int conn_sock = accept4(server_sock_fd, NULL, NULL, SOCK_NONBLOCK);
        if (conn_sock == -1) {
                return -1;
        }

        add_client(clients, conn_sock);
        async_add(epollfd, conn_sock, EPOLLIN | EPOLLET);
        info("Connection open: %d\n", conn_sock);

        err = write(conn_sock, CHAT_BANNER, sizeof(CHAT_BANNER));
        if (err == -1) {
                info("Error sending CHAT_BANNER to %d\n", conn_sock);
        }

        return 0;
}
