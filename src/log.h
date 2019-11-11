#ifndef __LOG_H__
#define __LOG_H__

#define TIME_FORMAT "%02d:%02d:%02d %.*s"
#define SIZE_TIME 9

void info(const char *fmt, ...);

char* log_format(const char *buffer, int size);

#endif
