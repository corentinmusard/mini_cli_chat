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
        int j = 0;
        int i_message = 0;
        char buffer_message[MAXMSG] = {0};
        WINDOW *messages_window = NULL; // to display messages received from server
        WINDOW *input_window = NULL; // to type new message

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

        while (!exit_wanted) {
                struct epoll_event events[MAX_EVENTS];
                int nfds;

                refresh_cli(messages_window, input_window);

                nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
                if (nfds == -1) {
                        perror("epoll_wait");
                        goto clean;
                }

                for (int i = 0; i < nfds; i++) {
                        if (events[i].data.fd == STDIN_FILENO) { //from stdin
                                char buffer[1] = {0};
                                if (read(STDIN_FILENO, buffer, 1) == -1) {
                                        perror("read");
                                        goto clean;
                                }

                                if (buffer[0] == '\r') { //end of the message, send it
                                        if (i_message == 0) { //blank message
                                                //don't send it
                                                continue;
                                        }
                                        if (buffer_message[0] == '/') { //start with '/'
                                                //It's a command
                                                if (!execute_command(buffer_message)) {
                                                        //command unknown
                                                        print_message(messages_window, ++j, "Command unknown\n");
                                                }
                                        } else if (write(sockfd, buffer_message, sizeof(buffer_message)) == -1) {
                                                perror("write");
                                                goto clean;
                                        }
                                        reset_variables(buffer_message, sizeof(buffer_message), &i_message);
                                        clear_message_area(input_window);
                                } else if (buffer[0] == 127) { //DEL
                                        delete_message_character(input_window, buffer_message, &i_message);
                                } else if (i_message == MAXMSG-1) { //max message length reached
                                        //ignore character for now
                                } else {
                                        buffer_message[i_message] = buffer[0];
                                        mvwprintw(input_window, 1, INITIAL_MESSAGE_X + i_message, "%c", buffer[0]);
                                        increment_indice_message(input_window, &i_message);
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
