#ifndef UTILS_UTILS_H
#define UTILS_UTILS_H
#ifdef __cplusplus
extern "C" {
#endif

#include <signal.h>

/**
 * server port
 */
#define PORT 5000

/**
 * Maximum length of a message
 */
#define MAXMSG 100

/**
 * Maximum events to be returned by epoll_wait
 * See man epoll_wait(2) for more infos
 */
#define MAX_EVENTS 10

/**
 * Default value is 0
 * If value is not 0 then client is going to exit
 */
extern volatile sig_atomic_t exit_wanted;

/**
 * Register signal SIGINT. When called, it changes a value of `exit_wanted`
 * Return -1 on failure and 0 on succes
 * errno is set by sigaction(2)
 */
int register_sigint(void);

#ifdef __cplusplus
}
#endif
#endif
