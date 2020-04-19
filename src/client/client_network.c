#include <arpa/inet.h>
#include <assert.h>
#include <sys/socket.h>
#include <unistd.h>

#include "client_network.h"

int connect_client(const char *ip, in_port_t port) {
        assert(ip && "should not be NULL");

        const struct sockaddr_in addr = {
                .sin_family = AF_INET,
                .sin_port = htons(port),
                .sin_addr.s_addr = inet_addr(ip)
        };
        int sockfd;
        int e;

        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) {
                return SOCKET_FAILED;
        }

        e = connect(sockfd, (const struct sockaddr *)&addr, sizeof(addr));
        if (e == -1) {
                return CONNECT_FAILED;
        }

        return sockfd;
}

void disconnect_client(int sockfd) {
        assert(sockfd >= 0 && "should be a valid file descriptor");
        close(sockfd);
}
