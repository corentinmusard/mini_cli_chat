#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <curses.h>
#include <signal.h>

#include "cli.h"
#include "client_lib.h"
#include "../utils/asynchronous.h"
#include "../utils/utils.h"

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
                perror("init_cli");
                goto clean;
        }

        while (!exit_wanted) {
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
                                char c;
                                if (read(STDIN_FILENO, &c, 1) == -1) {
                                        perror("read");
                                        goto clean;
                                }

                                if (c == '\r') { //end of the message, send it
                                        if (input->i == 0) { //blank message
                                                //don't send it
                                                continue;
                                        }
                                        if (input->buffer[0] == '/') { //start with '/'
                                                //It's a command
                                                if (!execute_command(input->buffer)) {
                                                        //command unknown
                                                        print_message(msgs, "Command unknown\n");
                                                }
                                        } else if (write(sockfd, input->buffer, (size_t)input->i) == -1) {
                                                perror("write");
                                                goto clean;
                                        }
                                        reset_variables(input);
                                        clear_message_area(input->window);
                                } else if (c == 127) { //DEL
                                        delete_message_character(input);
                                } else if (input->i == MAXMSG-1) { //max message length reached
                                        //ignore character for now
                                } else {
                                        input_char_handling(input, c);
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
