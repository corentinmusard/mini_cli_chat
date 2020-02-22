#include <stdio.h>
#include <sys/epoll.h>

#include "asynchronous.h"
#include "clients.h"
#include "server_lib.h"
#include "server_network.h"
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
                if (nfds == -1) {
                        err = -1;
                }

                for (int i = 0; i < nfds && err != 1; i++) {
                        if (events[i].data.fd == server_sock_fd) { //event from server socket
                                err = accept_client(clients, epollfd, server_sock_fd);
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
