#include <curses.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include "asynchronous.h"
#include "client_lib.h"
#include "screen.h"
#include "utils.h"

int main(void) {
        int epollfd;
        int sockfd;
        Screen *screen = screen_init();

        const struct sockaddr_in addr = {
                .sin_family = AF_INET,
                .sin_port = htons(PORT),
                .sin_addr.s_addr = INADDR_ANY
        };
        const struct sigaction act = {
                .sa_handler = int_handler,
                .sa_flags = SA_RESTART
        };

        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) {
                perror("socket");
                goto clean_fd;
        }

        if (sigaction(SIGINT, &act, NULL) == -1) {
                perror("sigaction");
                goto clean_fd;
        }

        if (connect(sockfd, (const struct sockaddr *)&addr, sizeof(addr)) == -1) {
                perror("connect");
                goto clean_fd;
        }

        epollfd = async_init();
        if (epollfd == -1) {
                perror("async_init");
                goto clean_fd;
        }

        if (register_event(epollfd, sockfd, EPOLLIN) == -1) {
                perror("register_event");
                goto clean_fd;
        }
        if (register_event(epollfd, STDIN_FILENO, EPOLLIN) == -1) {
                perror("register_event");
                goto clean_fd;
        }

        while (exit_wanted == 0) {
                struct epoll_event events[MAX_EVENTS];
                int nfds;

                refresh_screen(screen);

                nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
                if (nfds == -1) {
                        perror("epoll_wait");
                        goto clean;
                }

                for (int i = 0; i < nfds; i++) {
                        if (events[i].data.fd == STDIN_FILENO) {  // from stdin
                                if (stdin_char_handling(screen, sockfd) == -1) {
                                        goto clean;
                                }
                        } else if (events[i].data.fd == sockfd) {  // from server
                                if (server_message_handling(screen->msgs, sockfd) == -1) {
                                        goto clean;
                                }
                        } else {
                                fprintf(stderr, "unknown fd: %d\n", events[i].data.fd);
                                goto clean;
                        }
                }
        }

clean:
        endwin();

clean_fd:
        close(sockfd);
        free_screen(screen);
        return 0;
}
