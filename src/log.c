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
        struct tm *tm_s;
        time_t timep;

        timep = time(NULL);
        tm_s = localtime(&timep);

        printf(TIME_FORMAT, tm_s->tm_hour, tm_s->tm_min, tm_s->tm_sec);
}

void info(const char *fmt, ...) {
        va_list args;

        va_start(args, fmt);

        print_time();
        printf(" ");
        vprintf(fmt, args);

        va_end(args);
}

char* log_format(const char *buffer, size_t size) {
        struct tm *tm_s;
        time_t timep;
        char *message;

        //+1 for space between time and buffer
        message = malloc((size + SIZE_TIME + 1) * sizeof(char));

        timep = time(NULL);
        tm_s = localtime(&timep);

        sprintf(message, TIME_FORMAT " %.*s", tm_s->tm_hour, tm_s->tm_min, tm_s->tm_sec, (int)size, buffer);

        return message;
}
