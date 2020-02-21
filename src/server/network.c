#include <sys/socket.h>

#include "network.h"

int connect_server(in_port_t port) {
        const struct sockaddr_in addr = {
                .sin_family = AF_INET,
                .sin_port = htons(port),
                .sin_addr.s_addr = INADDR_ANY
        };
        int server_sock_fd;
        int e;

        server_sock_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
        if (server_sock_fd == -1) {
                return -1;
        }

        e = bind(server_sock_fd, (const struct sockaddr *) &addr, sizeof(addr));
        if (e == -1) {
                return -2;
        }

        e = listen(server_sock_fd, LISTEN_BACKLOG);
        if (e == -1) {
                return -3;
        }

        return server_sock_fd;
}
