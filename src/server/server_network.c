#include <assert.h>
#include <sys/socket.h>
#include <unistd.h>

#include "server_network.h"

int start_server(in_port_t port) {
        const struct sockaddr_in addr = {
                .sin_family = AF_INET,
                .sin_port = htons(port),
                .sin_addr.s_addr = INADDR_ANY
        };
        int server_sock_fd;
        int e;

        server_sock_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
        if (server_sock_fd == -1) {
                return SOCKET_FAILED;
        }

        e = bind(server_sock_fd, (const struct sockaddr *) &addr, sizeof(addr));
        if (e == -1) {
                return BIND_FAILED;
        }

        e = listen(server_sock_fd, LISTEN_BACKLOG);
        if (e == -1) {
                return LISTEN_FAILED;
        }

        return server_sock_fd;
}

void stop_server(int server_sock_fd) {
        assert(server_sock_fd >= 0 && "should be a valid file descriptor");
        close(server_sock_fd);
}
