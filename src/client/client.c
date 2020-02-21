#include <curses.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <unistd.h>

#include "asynchronous.h"
#include "client_lib.h"
#include "network.h"
#include "screen.h"
#include "utils.h"

int main(void) {
        int epollfd;
        int sockfd;
        Screen *screen = screen_init();

        sockfd = connect_client(INADDR_ANY, PORT);
        if (sockfd <= 0) {
                perror("connect_client");
                goto clean_fd;
        }

        if (register_sigint() == -1) {
                perror("register_sigint");
                goto clean_fd;
        }

        epollfd = async_init();
        if (epollfd == -1) {
                perror("async_init");
                goto clean_fd;
        }

        if (async_add(epollfd, sockfd, EPOLLIN) == -1) {
                perror("async_add");
                goto clean_fd;
        }
        if (async_add(epollfd, STDIN_FILENO, EPOLLIN) == -1) {
                perror("async_add");
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
