#ifndef TESTS_UTILS_H
#define TESTS_UTILS_H
#ifdef __cplusplus
extern "C" {
#endif

#include "server/clients.h"

/**
 * fd who doesn't exists
 */
#define BAD_FD 13371337

/**
 * Create a temporay file, return its file descriptor
 */
int get_fake_fd(void);

/**
 * Return a new socket
 */
int get_socket(void);

/**
 * Assert data read from `fd` equals to `buffer`
 */
void read_equal(int fd, const char *buffer);

/**
 * Assert data read from `fd` equals NOT to `buffer`
 */
void read_not_equal(int fd, const char *buffer);

/**
 * Fill `fd` with `buffer` of size `size`
 */
void fill_fake_fd(int fd, const char * buffer, size_t size);

/**
 * Replace stdin and fill it with `buffer`
 */
void fake_stdin(const char *buffer);

/**
 * Restore stdin after a call to fake_stdin
 */
void restore_stdin(void);

/**
 * Assert that clients list is empty
 */
void assert_is_empty(const Clients *clients);

/**
 * Assert that clients list are `fds` of size `size`
 */
void assert_fds_are(const Clients *clients, const int *fds, int size);

#ifdef __cplusplus
}
#endif
#endif
