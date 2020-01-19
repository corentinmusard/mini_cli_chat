#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#include "clients.h"
#include "utils.h"
#include "log.h"
#include "asynchronous.h"

static volatile sig_atomic_t sigintRaised = 0;

static void int_handler(int sig __attribute__ ((unused))) {
        sigintRaised = 1;
}

/**
 * Send message `buffer` of size `size` to each client in `clients_fd`
 */
static void broadcast_message(const clients *clients_fd, const char *buffer, size_t size) {
        const client *c;

        if (clients_fd == NULL || clients_fd->head == NULL) {
                return;
        }

        c = clients_fd->head;
        while (c != NULL) {
                write(c->fd, buffer, size);
                c = c->next;
        }
}

int main(void) {
        int epollfd;
        int server_sock_fd;
        clients *clients_fd = init_clients();
        const struct sockaddr_in addr = {
                .sin_family = AF_INET,
                .sin_port = htons(PORT),
                .sin_addr.s_addr = INADDR_ANY
        };
        const struct sigaction act = {
                .sa_handler = int_handler,
                .sa_flags = SA_RESTART
        };

        server_sock_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
        if (server_sock_fd == -1) {
                perror("socket");
                goto clean_server_fd;
        }

        if (sigaction(SIGINT, &act, NULL) == -1) {
                perror("sigaction");
                goto clean_server_fd;
        }

        if (bind(server_sock_fd, (const struct sockaddr *) &addr, sizeof(addr)) == -1) {
                perror("bind");
                goto clean_server_fd;
        }

        if (listen(server_sock_fd, LISTEN_BACKLOG) == -1) {
                perror("listen");
                goto clean_server_fd;
        }

        epollfd = async_init();
        if (epollfd == -1) {
                perror("async_init");
                goto clean_server_fd;
        }

        if (register_event(epollfd, server_sock_fd, EPOLLIN) == -1) {
                perror("register_event");
                goto clean_server_fd;
        }

        while (sigintRaised == 0) {
                struct epoll_event events[MAX_EVENTS];
                int nfds;

                nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
                if (nfds == -1 && errno != EINTR) {
                        perror("epoll_wait");
                        goto clean_server_fd;
                }

                for (int i = 0; i < nfds; i++) {
                        if (events[i].data.fd == server_sock_fd) { //event from server socket
                                int conn_sock = accept4(server_sock_fd, NULL, NULL, SOCK_NONBLOCK);
                                if (conn_sock == -1) {
                                        perror("accept4");
                                        goto clean_server_fd;
                                }
                                add_client(clients_fd, conn_sock);
                                register_event(epollfd, conn_sock, EPOLLIN | EPOLLET);
                                info("Connection open: %d\n", conn_sock);
                                if (write(conn_sock, CHAT_BANNER, sizeof(CHAT_BANNER)) == -1) {
                                        info("Error sending CHAT_BANNER to %d\n", conn_sock);
                                }
                        } else { //event from client socket
                                char buffer[MAXMSG] = {0};
                                ssize_t status = read(events[i].data.fd, buffer, sizeof(buffer));

                                if (status == -1) { //error
                                        perror("read");
                                        goto clean_server_fd;
                                } else if (status == 0) { //connection closed
                                        info("Connection closed: %d\n", events[i].data.fd);
                                        close(events[i].data.fd);
                                        delete_client(clients_fd, events[i].data.fd);
                                        continue;
                                } else {
                                        broadcast_message(clients_fd, buffer, sizeof(buffer));
                                        info("%.*s\n", MAXMSG, buffer);
                                }
                        }
                }
        }

clean_server_fd:
        close(server_sock_fd);
        free(clients_fd);
        return 0;
}
