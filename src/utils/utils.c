#include <assert.h>
#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <unistd.h>

#include "utils.h"

volatile sig_atomic_t interrupt = 0;

/**
 * Called when SIGINT is raised
 */
static void int_handler(int sig __attribute__((unused))) {
    want_to_exit();
}

int register_sigint(void) {
    const struct sigaction act = {
        .sa_handler = int_handler
    };
    return sigaction(SIGINT, &act, NULL);
}

void want_to_exit(void) {
    interrupt = 1;
}

bool exit_not_wanted(int err) {
    return err != -1 && interrupt == 0;
}

bool is_stdin(int fd) {
    assert(fd >= 0 && "should be a valid file descriptor");
    return fd == STDIN_FILENO;
}
