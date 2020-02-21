#include <signal.h>
#include <stddef.h>
#include <unistd.h>

#include "utils.h"

volatile sig_atomic_t interrupt = 0;

/**
 * Called when SIGINT is raised
 */
static void int_handler(int sig __attribute__((unused))) {
        interrupt = 1;
}

int register_sigint(void) {
        const struct sigaction act = {
                .sa_handler = int_handler,
                .sa_flags = SA_RESTART
        };
        return sigaction(SIGINT, &act, NULL);
}

int is_stdin(int fd) {
        return fd == STDIN_FILENO;
}