#ifndef LOG_H
#define LOG_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/**
 * It's like printf but fmt is preceded by TIME_FORMAT and a space
 */
void info(const char *fmt, ...);

/**
 * Return an malloc-ed char*
 * Return current time in TIME_FORMAT format followed by a space and `buffer` of size `size`
 */
char* log_format(const char *buffer, size_t size);

#ifdef __cplusplus
}
#endif
#endif
