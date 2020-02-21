#include <errno.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include "asynchronous.h"
#include "clients.h"
#include "log.h"
#include "network.h"
#include "server_lib.h"
#include "utils.h"

int main(void) {
        int epollfd;
        int server_sock_fd;
        Clients *clients_fd = init_clients();
        
        server_sock_fd = connect_server(PORT);
        if (server_sock_fd <= 0) {
                perror("connect_server");
                goto clean_server_fd;
        }

        if (register_sigint() == -1) {
                perror("register_sigint");
                goto clean_server_fd;
        }

        epollfd = async_init();
        if (epollfd == -1) {
                perror("async_init");
                goto clean_server_fd;
        }

        if (register_event(epollfd, server_sock_fd, EPOLLIN) == -1) {
                perror("register_event");
                goto clean_server_fd;
        }

        while (exit_wanted == 0) {
                struct epoll_event events[MAX_EVENTS];
                int nfds;

                nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
                if (nfds == -1 && errno != EINTR) {
                        perror("epoll_wait");
                        goto clean_server_fd;
                }

                for (int i = 0; i < nfds; i++) {
                        if (events[i].data.fd == server_sock_fd) { //event from server socket
                                int conn_sock = accept4(server_sock_fd, NULL, NULL, SOCK_NONBLOCK);
                                if (conn_sock == -1) {
                                        perror("accept4");
                                        goto clean_server_fd;
                                }
                                add_client(clients_fd, conn_sock);
                                register_event(epollfd, conn_sock, EPOLLIN | EPOLLET);
                                info("Connection open: %d\n", conn_sock);
                                if (write(conn_sock, CHAT_BANNER, sizeof(CHAT_BANNER)) == -1) {
                                        info("Error sending CHAT_BANNER to %d\n", conn_sock);
                                }
                        } else { //event from client socket
                                char buffer[MAXMSG] = {0};
                                ssize_t status = read(events[i].data.fd, buffer, sizeof(buffer));

                                if (status == -1) { //error
                                        perror("read");
                                        goto clean_server_fd;
                                } else if (status == 0) { //connection closed
                                        info("Connection closed: %d\n", events[i].data.fd);
                                        close(events[i].data.fd);
                                        delete_client(clients_fd, events[i].data.fd);
                                        continue;
                                } else {
                                        broadcast_message(clients_fd, buffer, sizeof(buffer));
                                        info("%.*s\n", MAXMSG, buffer);
                                }
                        }
                }
        }

clean_server_fd:
        close(server_sock_fd);
        free_clients(clients_fd);
        return 0;
}
