#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>

#include "asynchronous.h"

int async_init(void) {
    int epollfd = epoll_create1(0);
    if (epollfd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }
    return epollfd;
}

void async_add(int epollfd, int fd, uint32_t events) {
    assert(epollfd >= 0 && "should be a valid file descriptor");
    assert(fd >= 0 && "should be a valid file descriptor");

    struct epoll_event ev = {
        .events = events,
        .data.fd = fd
    };
    int e = epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
    if (e == -1) {
        perror("epoll_ctl");
        exit(EXIT_FAILURE);
    }
}

int wait_events(int epollfd, struct epoll_event *events) {
    assert(epollfd >= 0 && "should be a valid file descriptor");

    int n = epoll_wait(epollfd, events, MAX_EVENTS, -1);
    if (n == -1) {
        perror("epoll_wait");
        exit(EXIT_FAILURE);
    }
    return n;
}

/**
 * https://stackoverflow.com/questions/1543466/how-do-i-change-a-tcp-socket-to-be-non-blocking
 */
void make_fd_non_blocking(int fd) {
    assert(fd >= 0 && "should be a valid file descriptor");

    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
}
