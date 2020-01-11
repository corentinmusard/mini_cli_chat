#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "log.h"

#define TIME_FORMAT "%02d:%02d:%02d"
#define SIZE_TIME 8

/**
 * Print current time to stdout in TIME_FORMAT format
 */
static void print_time(void) {
        struct tm tm_s;
        time_t timep;

        timep = time(NULL);
        localtime_r(&timep, &tm_s);

        printf(TIME_FORMAT, tm_s.tm_hour, tm_s.tm_min, tm_s.tm_sec);
}

__attribute__((__format__ (__printf__, 1, 2)))
void info(const char *fmt, ...) {
        va_list args;

        va_start(args, fmt);

        print_time();
        printf(" ");
        vprintf(fmt, args);

        va_end(args);
}

char* log_format(const char *buffer, size_t size) {
        struct tm tm_s;
        time_t timep;
        char *message;
        size_t m_size = size + SIZE_TIME + 1;

        //+1 for space between time and buffer
        message = calloc(m_size, sizeof(char));

        timep = time(NULL);
        localtime_r(&timep, &tm_s);

        snprintf(message, m_size, TIME_FORMAT " %.*s", tm_s.tm_hour, tm_s.tm_min, tm_s.tm_sec, (int)size, buffer);

        return message;
}
