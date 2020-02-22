#include <errno.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <sys/socket.h>

#include "asynchronous.h"
#include "clients.h"
#include "network.h"
#include "server_lib.h"
#include "utils.h"

int main(void) {
        Clients *clients = init_clients();
        int epollfd;
        int server_sock_fd;
        int err = 0;

        server_sock_fd = start_server(PORT);
        if (server_sock_fd < 0) {
                perror("start_server");
                goto clean_server_fd;
        }

        if (register_sigint() == -1) {
                perror("register_sigint");
                goto clean_server_fd;
        }

        epollfd = server_async_init(server_sock_fd);
        if (epollfd == -1) {
                perror("server_async_init");
                goto clean_server_fd;
        }

        while (exit_not_wanted(err)) {
                struct epoll_event events[MAX_EVENTS];
                int nfds;

                nfds = wait_events(epollfd, events);
                if (nfds == -1 && errno != EINTR) {
                        perror("epoll_wait");
                        goto clean_server_fd;
                }

                for (int i = 0; i < nfds && err != 1; i++) {
                        if (events[i].data.fd == server_sock_fd) { //event from server socket
                                int conn_sock = accept4(server_sock_fd, NULL, NULL, SOCK_NONBLOCK);
                                if (conn_sock == -1) {
                                        perror("accept4");
                                        goto clean_server_fd;
                                }
                                err = accept_client(clients, epollfd, conn_sock);
                        } else { //event from client socket
                                err = client_message_handling(clients, events[i].data.fd);
                        }
                }
        }

clean_server_fd:
        stop_server(server_sock_fd);
        free_clients(clients);
        return 0;
}
