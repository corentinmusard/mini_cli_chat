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
        int server_fd;
        int nfds = 0;

        server_fd = start_server(PORT);
        if (server_fd < 0) {
                perror("start_server");
                goto clean;
        }

        if (register_sigint() == -1) {
                perror("register_sigint");
                goto clean;
        }

        epollfd = server_async_init(server_fd);
        if (epollfd == -1) {
                perror("server_async_init");
                goto clean;
        }

        while (exit_not_wanted(nfds)) {
                struct epoll_event events[MAX_EVENTS];
                nfds = wait_events(epollfd, events);

                for (int i = 0; i < nfds && nfds != -1; i++) {
                        if (events[i].data.fd == server_fd) { //event from server socket
                                accept_client(clients, epollfd, server_fd);
                        } else { //event from client socket
                                client_message_handling(clients, events[i].data.fd);
                        }
                }
        }

clean:
        stop_server(server_fd);
        free_clients(clients);
        return 0;
}
