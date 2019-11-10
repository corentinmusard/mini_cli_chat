#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <signal.h>

#include "cli.h"
#include "asynchronous.h"
#include "utils.h"
#include "log.h"

volatile sig_atomic_t sigintRaised = 0;

void int_handler(int sig) {
        sigintRaised = 1;
}

int main(void)
{
        struct epoll_event events[MAX_EVENTS];
        int nfds, epollfd;
        int sockfd;
        int j = 3, i_message = 0;
        char buffer_message[MAXMSG] = {0};
        WINDOW *messages_window = NULL, *input_window = NULL;

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

        init_cli(&messages_window, &input_window);
        if (messages_window == NULL || input_window == NULL) {
                perror("init_cli");
                goto clean;
        }
        
        while (1) {
                refresh_cli(messages_window, input_window);
                nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
                if (nfds == -1) {
                        perror("epoll_wait");
                        goto clean;
                }

                for (int i = 0; i < nfds; i++) {
                        if (events[i].data.fd == STDIN_FILENO) { //from stdin
                                char buffer[1] = {0};
                                if (read(events[i].data.fd, buffer, 1) == -1) {
                                        perror("read");
                                        goto clean;
                                }

                                if (buffer[0] == '\r') { //end of the message, send it
                                        if (send(sockfd, buffer_message, sizeof(buffer_message), 0) == -1) {
                                                perror("send");
                                                goto clean;
                                        }
                                        memset(buffer_message, 0, sizeof(buffer_message));
                                        i_message = 0;
                                        clrtoeol();
                                        wmove(input_window, 1, 4);
                                        continue;
                                } else if (buffer[0] == 127) { //DEL
                                        buffer_message[i_message] = '\0';
                                        i_message--;
                                        if (i_message < 0) {
                                                i_message = 0;
                                        }
                                        wmove(input_window, 1, i_message+4);
                                        wdelch(input_window);
                                        continue;
                                }
                                if (i_message == MAXMSG-1) { //max message length reached
                                        continue; //ignore character
                                }
                                buffer_message[i_message] = buffer[0];
                                mvwprintw(input_window, 1, i_message+4, "%c", buffer[0]);
                                i_message++;
                                if (i_message > getmaxx(input_window)-4) {
                                        i_message = getmaxx(input_window)-4;
                                }
                        } else if (events[i].data.fd == sockfd) { //from server
                                char buffer[MAXMSG] = {0};
                                char *formated_message;
                                long int status = recv(events[i].data.fd, buffer, sizeof(buffer), 0);
                                if (status == -1) {
                                        perror("recv");
                                        goto clean;
                                } else if (status == 0) { //connection to server closed
                                        perror("recv");
                                        goto clean;
                                }

                                formated_message = log_format(buffer, sizeof(buffer));
                                mvwprintw(messages_window, j, 1, formated_message);
                                j++;
                                free(formated_message);
                        } else {
                                perror("fd");
                                goto clean;
                        }
                }  
        }

        clean:
        endwin();

        clean_fd:
        close(sockfd);

        return 0;
}