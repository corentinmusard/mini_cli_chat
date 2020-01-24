#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <curses.h>
#include <signal.h>

#include "cli.h"
#include "client_lib.h"
#include "asynchronous.h"
#include "utils.h"

int main(void) {
        int epollfd;
        int sockfd;
        Messages *msgs = messages_init();
        Input *input = input_init();

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

        if (connect(sockfd, (const struct sockaddr *) &addr, sizeof(addr)) == -1) {
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

        init_cli(&msgs->window, &input->window);
        if (msgs->window == NULL || input->window == NULL) {
                fprintf(stderr, "init_cli: Failed\n");
                goto clean;
        }

        while (exit_wanted == 0) {
                struct epoll_event events[MAX_EVENTS];
                int nfds;

                refresh_cli(msgs->window, input->window);

                nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
                if (nfds == -1) {
                        perror("epoll_wait");
                        goto clean;
                }

                for (int i = 0; i < nfds; i++) {
                        if (events[i].data.fd == STDIN_FILENO) { //from stdin
                                if (stdin_char_handling(input, msgs, sockfd) == -1) {
                                        goto clean;
                                }
                        } else if (events[i].data.fd == sockfd) { //from server
                                if (server_message_handling(msgs, sockfd) == -1) {
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

        free_input(input);
        free_messages(msgs);
        return 0;
}
