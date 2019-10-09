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

#include "utils.h"
#include "log.h"
#include "asynchronous.h"

int main(void)
{
        struct epoll_event events[MAX_EVENTS];
        int conn_sock, nfds, epollfd;
        int sockfd;
        const struct sockaddr_in addr = {
                .sin_family = AF_INET,
                .sin_port = htons(PORT),
                .sin_addr.s_addr = INADDR_ANY
        };

        sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
        if (sockfd == -1) {
                handle_error("socket");
        }

        if (bind(sockfd, (const struct sockaddr *) &addr, sizeof(addr)) == -1) {
                handle_error("bind");
        }

        if (listen(sockfd, LISTEN_BACKLOG) == -1) {
                handle_error("listen");
        }

        epollfd = async_init();
        register_event(epollfd, sockfd, EPOLLIN);

        while (1) {
                nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
                if (nfds == -1) {
                        handle_error("epoll_wait");
                }

                for (int i = 0; i < nfds; i++) {
                        if (events[i].data.fd == sockfd) { //event from server socket
                                conn_sock = accept4(sockfd, NULL, NULL, SOCK_NONBLOCK);
                                if (conn_sock == -1) {
                                        handle_error("accept");
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
                                        handle_error("recv");
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

        if (close(sockfd) == -1) {
                handle_error("close");
        }

        return 0;
}