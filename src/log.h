#ifndef LOG_H
#define LOG_H

#define TIME_FORMAT "%02d:%02d:%02d %.*s"
#define SIZE_TIME 9

void info(const char *fmt, ...);

char* log_format(const char *buffer, int size);

#endif
