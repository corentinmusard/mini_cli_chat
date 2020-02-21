#ifndef UTILS_ASYNCHRONOUS_H
#define UTILS_ASYNCHRONOUS_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

struct epoll_event; // defined in sys/epoll.h

/**
 * Maximum events to be returned by epoll_wait
 * See man epoll_wait(2) for more infos
 */
#define MAX_EVENTS 10

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
int async_add(int epollfd, int fd, uint32_t events);

/**
 * Wait until one or more event is received by epollfd
 * Return how many events are found
 * Return -1 on failure
 */
int wait_events(int epollfd, struct epoll_event *events);

#ifdef __cplusplus
}
#endif
#endif
