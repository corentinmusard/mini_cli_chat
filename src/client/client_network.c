#include <arpa/inet.h>
#include <assert.h>
#include <sys/socket.h>
#include <unistd.h>

#include "client_network.h"

int connect_client(const char *ip, in_port_t port) {
    assert(ip && "should not be NULL");

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        return SOCKET_FAILED;
    }

    struct in_addr ip_addr;
    if (inet_pton(AF_INET, ip, &ip_addr) != 1) { // IPv4 only
        return BAD_IP;
    }

    const struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr = ip_addr
    };
    int e = connect(sockfd, (const struct sockaddr *)&addr, sizeof(addr));
    if (e == -1) {
        return CONNECT_FAILED;
    }

    return sockfd;
}

void disconnect_client(int sockfd) {
    assert(sockfd >= 0 && "should be a valid file descriptor");
    close(sockfd);
}
