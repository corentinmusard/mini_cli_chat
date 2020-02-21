#include <sys/socket.h>

#include "network.h"

int connect_client(uint32_t ip, in_port_t port) {
        const struct sockaddr_in addr = {
                .sin_family = AF_INET,
                .sin_port = htons(port),
                .sin_addr.s_addr = ip
        };
        int sockfd;

        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) {
                return -1;
        }

        int e = connect(sockfd, (const struct sockaddr *)&addr, sizeof(addr));
        if (e == -1) {
                return -2;
        }

        return sockfd;
}
