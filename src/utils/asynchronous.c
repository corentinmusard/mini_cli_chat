#include <sys/epoll.h>

#include "asynchronous.h"

int async_init(void) {
        return epoll_create1(0);
}

int async_add(int epollfd, int fd, uint32_t events) {
        struct epoll_event ev = {
                .events = events,
                .data.fd = fd
        };
        return epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}

int wait_events(int epollfd, struct epoll_event *events) {
        return epoll_wait(epollfd, events, MAX_EVENTS, -1);
}
