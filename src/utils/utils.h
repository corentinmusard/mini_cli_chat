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
 * Maximum message length sent by a client
 */
#define MAXMSG_CLI 100

/**
 * Maximum message length sent by the server
 */
#define MAXMSG_SERV 150

/**
 * Max username length
 */
#define MAX_USERNAME_LENGTH 20

/**
 * Default value is 0
 * If value is not 0 then client or server is going to exit
 */
extern volatile sig_atomic_t interrupt;

/**
 * Register signal SIGINT. When called, it changes the value of `interrupt`
 * Return -1 on failure and 0 on succes
 * errno is set by sigaction(2)
 */
int register_sigint(void);

/**
 * client or server is going to exit
 */
void want_to_exit(void);

/**
 * Returne true if error code means failure or sigint has bean triggered.
 */
bool exit_not_wanted(int err);

/**
 * Return true if fd is STDIN_FILENO
 */
bool is_stdin(int fd);

#ifdef __cplusplus
}
#endif
#endif
