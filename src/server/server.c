#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>

#include "asynchronous.h"
#include "clients.h"
#include "server_lib.h"
#include "server_network.h"
#include "utils.h"

int main(void) {
    if (register_sigint() == -1) {
        perror("register_sigint");
        return EXIT_FAILURE;
    }

    int server_fd = start_server(PORT);
    if (server_fd < 0) {
        perror("start_server");
        return EXIT_FAILURE;
    }

    int epollfd = server_async_init(server_fd);
    Clients *clients = init_clients();

    int nfds = 0;
    while (exit_not_wanted(nfds)) {
        struct epoll_event events[MAX_EVENTS];
        nfds = wait_events(epollfd, events);

        for (int i = 0; i < nfds && nfds != -1; i++) {
            if (events[i].data.fd == server_fd) { //event from server socket
                accept_client(clients, epollfd, server_fd);
            } else { //event from client socket
                Client *c = get_client(clients, events[i].data.fd);
                client_message_handling(c);
            }
        }
    }

    free_clients(clients);
    stop_server(server_fd);
    return EXIT_SUCCESS;
}
