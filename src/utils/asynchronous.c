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
    int n = epoll_wait(epollfd, events, MAX_EVENTS, -1);
    if (n == -1) {
        perror("epoll_wait");
        exit(EXIT_FAILURE);
    }
    return n;
}
