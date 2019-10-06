#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <time.h>

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
        int j = 3, i_message = 0;
        char buffer_message[MAXMSG] = {0};
        WINDOW *messages_window, *input_window;

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

        initscr();
        messages_window = subwin(stdscr, LINES-2, COLS, 0, 0);
        input_window = subwin(stdscr, 2, COLS, LINES-2, 0);
        mvwprintw(messages_window, 1, 1, "Ceci est la fenetre du messages_window\n");
        mvwprintw(messages_window, 2, 1, "Salut toi\n");
        mvwprintw(input_window, 1, 1, ">> ");
        box(messages_window, ACS_VLINE, ACS_HLINE);
        move(LINES-1, 4);
        
        while(1) {
                wrefresh(messages_window);
                wrefresh(input_window);
                nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
                if (nfds == -1) {
                        handle_error("epoll_wait");
                }

                for (int i = 0; i < nfds; i++) {
                        if (events[i].data.fd == STDIN_FILENO) { //from stdin
                                char buffer[1] = {0};
                                if (read(events[i].data.fd, buffer, 1) == -1) {
                                        handle_error("read");
                                }

                                if (buffer[0] == '\r') { //end of the message, send it
                                        if (send(sockfd, buffer_message, sizeof(buffer_message), 0) == -1) {
                                                handle_error("send");
                                        }
                                        memset(buffer_message, 0, sizeof(buffer_message));
                                        i_message = 0;
                                        clrtoeol();
                                        wmove(input_window, LINES-1, 4);
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
                                buffer_message[i_message] = buffer[0];
                                mvwprintw(input_window, 1, i_message+4, "%c", buffer[0]);
                                i_message++;
                        } else if (events[i].data.fd == sockfd) { //from server
                                char buffer[MAXMSG] = {0};
                                time_t timep;
                                struct tm *tm_s;
                                long int status = recv(events[i].data.fd, buffer, sizeof(buffer), 0);
                                if (status == -1) {
                                        handle_error("recv");
                                }
                                else if (status == 0) { //connection to server closed
                                        handle_error("recv");
                                }

                                timep = time(NULL);
                                tm_s = localtime(&timep);
                                mvwprintw(messages_window, j, 1, "%02d:%02d:%02d %s", tm_s->tm_hour,
                                                                tm_s->tm_min, tm_s->tm_sec, buffer);
                                j++;
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