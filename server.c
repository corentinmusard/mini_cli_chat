#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>

#include "utils.h"
#include "log.h"
#include "asynchronous.h"

volatile sig_atomic_t sigintRaised = 0;

void int_handler(int sig) {
        sigintRaised = 1;
}

int main(void)
{
        struct epoll_event events[MAX_EVENTS];
        int conn_sock, nfds, epollfd;
        int server_sock_fd;
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

        while (!sigintRaised) {
                nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
                if (nfds == -1 && errno != EINTR) {
                        perror("epoll_wait");
                        goto clean;
                }

                for (int i = 0; i < nfds; i++) {
                        if (events[i].data.fd == server_sock_fd) { //event from server socket
                                conn_sock = accept4(server_sock_fd, NULL, NULL, SOCK_NONBLOCK);
                                if (conn_sock == -1) {
                                        perror("accept");
                                        goto clean;
                                }
                                register_event(epollfd, conn_sock, EPOLLIN | EPOLLET);
                                info("Connection open: %d\n", conn_sock);
                                /*if (send(conn_sock, CHAT_BANNER, sizeof(CHAT_BANNER), 0) == -1) {
                                        printf("send1");
                                        exit(1);
                                }
                                sleep(2);
                                char m2[] = "You're still connected";
                                if (send(conn_sock, m2, sizeof(m2), 0) == -1) {
                                        printf("send2");
                                        exit(1);
                                }*/

                        } else { //event from client socket
                                char buffer[MAXMSG] = {0};
                                long int status = recv(events[i].data.fd, buffer, sizeof(buffer), 0);

                                if (status == -1) { //error
                                        perror("recv");
                                        goto clean;
                                } else if (status == 0) { //connection closed
                                        info("Connection closed: %d\n", events[i].data.fd);
                                        close(events[i].data.fd);
                                        continue;
                                }
                                //else
                                //send(events[i].data.fd, buffer, sizeof(buffer), 0);
                                info("%.*s\n", MAXMSG, buffer);
                        }
                }
        }

        clean:
        for (int i = 0; i < MAX_EVENTS; i++) { //close each sockets still open
                close(events[i].data.fd);
        }

        clean_server_fd:
        close(server_sock_fd);

        return 0;
}