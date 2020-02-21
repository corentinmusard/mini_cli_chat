#include <curses.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <unistd.h>

#include "client_lib.h"
#include "network.h"
#include "screen.h"
#include "utils.h"

int main(void) {
        int epollfd;
        int sockfd;
        Screen *screen = screen_init();

        sockfd = connect_client("127.0.0.1", PORT);
        if (sockfd <= 0) {
                perror("connect_client");
                goto clean_fd;
        }

        if (register_sigint() == -1) {
                perror("register_sigint");
                goto clean_fd;
        }

        epollfd = client_async_init(sockfd);
        if (epollfd == -1) {
                perror("client_async_init");
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
                        int err;
                        if (events[i].data.fd == STDIN_FILENO) {  // from stdin
                                err = stdin_char_handling(screen, sockfd);
                        } else if (events[i].data.fd == sockfd) {  // from server
                                err = server_message_handling(screen->msgs, sockfd);
                        } else {
                                fprintf(stderr, "unknown fd: %d\n", events[i].data.fd);
                                err = -1;
                        }

                        if (err == -1) {
                                exit_wanted = 1;
                                break;
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
