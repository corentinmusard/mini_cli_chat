#include <assert.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "log.h"

/**
 * Time format
 * hour:min:sec
 */
#define TIME_FORMAT "%02d:%02d:%02d"

/**
 * size of TIME_FORMAT + 1 for '\0'
 */
#define SIZE_TIME (8 + 1)

/**
 * The file to write log to
 */
static FILE *log_file = NULL;

/**
 * Store locale time into `tm_s`
 */
static void get_localtime(struct tm *tm_s) {
    assert(tm_s && "should not be NULL");

    time_t timep = time(NULL);
    localtime_r(&timep, tm_s);
}

/**
 * Print current time to stdout in TIME_FORMAT format
 */
static void print_time(void) {
    struct tm tm_s;
    get_localtime(&tm_s);

    fprintf(log_file, TIME_FORMAT, tm_s.tm_hour, tm_s.tm_min, tm_s.tm_sec);
}

__attribute__((__format__ (__printf__, 1, 2)))
void info(const char *fmt, ...) {
    assert(fmt && "should not be NULL");

    // On the first time, initialize log_file
    if (log_file == NULL) {
        set_logfile(NULL);
    }

    print_time();
    fprintf(log_file, " ");

    va_list args;
    va_start(args, fmt);
    vfprintf(log_file, fmt, args);
    va_end(args);

    fflush(log_file);
}

char* log_format(const char *buffer, size_t size) {
    assert(buffer && "should not be NULL");
    assert(size > 0 && "should not be 0");

    // +1 for space between time and buffer
    // +1 for '\0'
    size_t m_size = SIZE_TIME + 1 + size + 1;
    char *message = calloc(m_size, sizeof(char));

    struct tm tm_s;
    get_localtime(&tm_s);

    int len = snprintf(message, m_size, TIME_FORMAT " %s", tm_s.tm_hour, tm_s.tm_min, tm_s.tm_sec, buffer);
    assert(len > 0 && "snprintf return a positive number");
    if ((size_t)len >= m_size) {
        info("%s: truncated output: len=%d, m_size=%zu\n", __func__, len, m_size);
    }

    return message;
}

void set_logfile(const char *filename) {
    if (filename == NULL) {
        unset_logfile();
        return;
    }

    FILE *f = fopen(filename, "a");
    if (f == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    unset_logfile();
    log_file = f;
}

void unset_logfile(void) {
    if (log_file != NULL) {
        fclose(log_file);
    }
    log_file = stderr;
}

ssize_t log_and_write(int fd, const void *buf, size_t count) {
    assert(fd >= 0 && "should be a valid file descriptor");
    assert(fd <= INT_MAX && "should be a valid int");
    assert(buf && "should not be NULL");
    assert(count > 0 && "should not be 0");

    info("%.*s\n", (int)count, (const char *)buf);
    return write(fd, buf, count);
}
