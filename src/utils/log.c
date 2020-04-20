#include <assert.h>
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

    printf(TIME_FORMAT, tm_s.tm_hour, tm_s.tm_min, tm_s.tm_sec);
}

__attribute__((__format__ (__printf__, 1, 2)))
void info(const char *fmt, ...) {
    assert(fmt && "should not be NULL");

    print_time();
    printf(" ");

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

char* log_format(const char *buffer, size_t size) {
    assert(buffer && "should not be NULL");
    assert(size > 0 && "should not be 0");

    size_t m_size = SIZE_TIME + 1 + size; //+1 for space between time and buffer
    char *message = calloc(m_size, sizeof(char));

    struct tm tm_s;
    get_localtime(&tm_s);

    snprintf(message, m_size, TIME_FORMAT " %s", tm_s.tm_hour, tm_s.tm_min, tm_s.tm_sec, buffer);
    return message;
}
