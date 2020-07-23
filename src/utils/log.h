#ifndef UTILS_LOG_H
#define UTILS_LOG_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <unistd.h>

/**
 * It's like printf but fmt is preceded by TIME_FORMAT and a space
 * Write to logfile.
 */
void info(const char *fmt, ...);

/**
 * Return an malloc-ed char*
 * Return current time in TIME_FORMAT format followed by a space and `buffer` of size `size`
 */
char* log_format(const char *buffer, size_t size);

/**
 * Open `filename` if not NULL else set it to stderr.
 * It will be the log file
 */
void set_logfile(const char *filename);

/**
 * Unset logfile, logs will now go to stderr
 */
void unset_logfile(void);

/**
 * Log `buf` of len `count`, then call and return write(2).
 */
ssize_t log_and_write(int fd, const void *buf, size_t count);

#ifdef __cplusplus
}
#endif
#endif
