#include <sys/epoll.h>
#include <stdint.h>

#include "asynchronous.h"

int async_init(void) {
        return epoll_create1(0);
}

int register_event(int epollfd, int fd, uint32_t events) {
        struct epoll_event ev = {
                .events = events,
                .data.fd = fd
        };
        return epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}