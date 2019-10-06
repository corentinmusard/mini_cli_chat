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

#define LISTEN_BACKLOG 50
#define PORT 5000
#define MAX_EVENTS 10
#define MAXMSG 100
#define CHAT_BANNER "Welcome to the basic chat!"

#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(void)
{
        struct epoll_event ev, events[MAX_EVENTS];
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

        epollfd = epoll_create1(0);
        if (epollfd == -1) {
                handle_error("epoll_create1");
        }

        ev.events = EPOLLIN;
        ev.data.fd = sockfd;
        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev) == -1) {
                handle_error("epoll_ctl: sockfd");
        }

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
                                ev.events = EPOLLIN | EPOLLET;
                                ev.data.fd = conn_sock;
                                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1) {
                                        handle_error("epoll_ctl: conn_sock");
                                }
                                printf("Connection open: %d\n", conn_sock);
                        } else { //event from client socket
                                char buffer[MAXMSG] = {0};
                                long int status = recv(events[i].data.fd, buffer, sizeof(buffer), 0);

                                if (status == -1) { //error
                                        handle_error("recv");
                                } else if (status == 0) { //connection closed
                                        printf("Connection closed: %d\n", events[i].data.fd);
                                        close(events[i].data.fd);
                                        continue;
                                }
                                //else
                                printf("Received: %s\n", buffer);
                                send(events[i].data.fd, buffer, sizeof(buffer), 0);
                                printf("Sent: %s\n", buffer);
                        }
                }
        }

        if (close(sockfd) == -1) {
                handle_error("close");
        }

        return 0;
}