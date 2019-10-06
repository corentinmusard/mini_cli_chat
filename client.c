#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#define PORT 5000
#define MAXMSG 100
#define MAX_EVENTS 10

#define handle_error(msg) \
           do { perror(msg); endwin(); exit(EXIT_FAILURE); } while (0)

int main(void)
{
        struct epoll_event ev, events[MAX_EVENTS];
        int nfds, epollfd;
        int sockfd;
       // char buffer[100] = {0};

        const struct sockaddr_in addr = {
                .sin_family = AF_INET,
                .sin_port = htons(PORT),
                .sin_addr.s_addr = INADDR_ANY
        };

        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) {
                handle_error("socket");
        }

        if (connect(sockfd, (const struct sockaddr *) &addr, sizeof(addr)) == -1) {
                handle_error("connect");
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

        ev.events = EPOLLIN;
        ev.data.fd = STDIN_FILENO;
        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev) == -1) {
                handle_error("epoll_ctl: STDIN_FILENO");
        }

        initscr(); //init screen
        printw("Hello World");
        refresh();
        getch();

        while(1) {
                printf("Envoyer >> ");
                nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
                if (nfds == -1) {
                        handle_error("epoll_wait");
                }

                for (int i = 0; i < nfds; i++) {
                        if (events[i].data.fd == STDIN_FILENO) { //from stdin
                                char buffer[MAXMSG] = {0};
                                fgets(buffer, MAXMSG, stdin);
                                if (send(sockfd, buffer, sizeof(buffer), 0) == -1) {
                                        handle_error("send");
                                }
                                printf("Fait.\n");
                        } else if (events[i].data.fd == sockfd) { //from server
                                char buffer[MAXMSG] = {0};
                                if (recv(events[i].data.fd, buffer, sizeof(buffer), 0) == -1) {
                                        handle_error("recv");
                                }
                                printf("Received: %s\n", buffer);
                        } else {
                                handle_error("fd");
                        }
                }

                
        }


        if (close(sockfd) == -1) {
                handle_error("close");
        }

        endwin();
        return 0;
}