#ifndef UTILS_UTILS_H
#define UTILS_UTILS_H
#ifdef __cplusplus
extern "C" {
#endif

#include <signal.h>
#include <stdbool.h>

/**
 * server port
 */
#define PORT 5000

/**
 * Maximum length of a message
 */
#define MAXMSG 100

/**
 * Default value is 0
 * If value is not 0 then client is going to exit
 */
extern volatile sig_atomic_t interrupt;

/**
 * Register signal SIGINT. When called, it changes a value of `interrupt`
 * Return -1 on failure and 0 on succes
 * errno is set by sigaction(2)
 */
int register_sigint(void);

/**
 * Return true if fd is STDIN_FILENO
 */
bool is_stdin(int fd);

#ifdef __cplusplus
}
#endif
#endif
