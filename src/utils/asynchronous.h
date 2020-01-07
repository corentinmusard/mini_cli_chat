#ifndef ASYNCHRONOUS_H
#define ASYNCHRONOUS_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * Return a new epoll(7) instance
 */
int async_init(void);

/**
 * Register `fd` on the epoll instance `epollfd` and associate it with `events`
 * Return 0 on succes
 * Return -1 on failure and errno is set by epoll_ctl
 * See man EPOLL_CTL(2) for more infos
 */
int register_event(int epollfd, int fd, uint32_t events);

#ifdef __cplusplus
}
#endif
#endif
