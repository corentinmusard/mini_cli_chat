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

static void int_handler(int sig __attribute__ ((unused))) {
        sigintRaised = 1;
}

static int server_message_handling(WINDOW *messages_window, int sockfd, int j) {
        char buffer[MAXMSG] = {0};
        char *formated_message;
        long int status = recv(sockfd, buffer, sizeof(buffer), 0);
        if (status == -1) {
                perror("recv");
                return -1;
        } else if (status == 0) { //connection to server closed
                perror("recv");
                return -1;
        }

        formated_message = log_format(buffer, sizeof(buffer));
        mvwprintw(messages_window, j, 1, formated_message);
        free(formated_message);
        return 0;
}

static void delete_message_character(WINDOW *input_window, char *buffer_message, int *i_message) {
        buffer_message[*i_message] = '\0';
        (*i_message)--;
        if (*i_message < 0) {
                *i_message = 0;
        }
        wmove(input_window, 1, *i_message+INITIAL_MESSAGE_X);
        wdelch(input_window);
}

static void update_indice_message(const WINDOW *input_window, int *i_message) {
        int max_x = getmaxx(input_window)-INITIAL_MESSAGE_X;

        (*i_message)++;
        if (*i_message > max_x) {
                *i_message = max_x;
        }
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
        
        while (!sigintRaised) {
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
                                        wmove(input_window, 1, INITIAL_MESSAGE_X);
                                } else if (buffer[0] == 127) { //DEL
                                        delete_message_character(input_window, buffer_message, &i_message);
                                } else if (i_message == MAXMSG-1) { //max message length reached
                                        //ignore character
                                } else {
                                        buffer_message[i_message] = buffer[0];
                                        mvwprintw(input_window, 1, i_message+INITIAL_MESSAGE_X, "%c", buffer[0]);
                                        update_indice_message(input_window, &i_message);
                                }
                        } else if (events[i].data.fd == sockfd) { //from server
                                int e = server_message_handling(messages_window, sockfd, ++j);
                                if (e == -1) {
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

        return 0;
}