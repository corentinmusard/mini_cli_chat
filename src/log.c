#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "log.h"

void info(const char *fmt, ...) {
        va_list args;
        va_start(args, fmt);

        //TODO
        /*time_t timep = time(NULL);
        struct tm *tm_s = localtime(&timep);
        char *str_time = malloc(8* sizeof(char));
        printf("%02d:%02d:%02d", tm_s->tm_hour, tm_s->tm_min, tm_s->tm_sec);
        */

        vprintf(fmt, args);
        va_end(args);
}

char* log_format(const char *buffer, int size) {
        struct tm *tm_s;
        time_t timep;
        char *message;

        message = malloc((long unsigned int)size+SIZE_TIME * sizeof(char));
        if (message == NULL) {
                return NULL;
        }

        timep = time(NULL);
        tm_s = localtime(&timep);

        sprintf(message, TIME_FORMAT, tm_s->tm_hour, tm_s->tm_min, tm_s->tm_sec, size, buffer);

        return message;
}
