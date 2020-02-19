#include <signal.h>
#include <stddef.h>

#include "utils.h"

volatile sig_atomic_t exit_wanted = 0;

/**
 * Called when SIGINT is raised
 */
static void int_handler(int sig __attribute__((unused))) {
        exit_wanted = 1;
}

int register_sigint(void) {
        const struct sigaction act = {
                .sa_handler = int_handler,
                .sa_flags = SA_RESTART
        };
        return sigaction(SIGINT, &act, NULL);
}
