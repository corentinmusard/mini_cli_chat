#include <sys/epoll.h>

#include "asynchronous.h"
#include "utils.h"

int async_init(void) {
        int epollfd = epoll_create1(0);
        if (epollfd == -1) {
                return -1;
        }
        return epollfd;
}

int register_event(int epollfd, int fd, unsigned int events) {
        struct epoll_event ev;
        ev.events = events;
        ev.data.fd = fd;
        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
                return -1;
        }
        return 0;
}
