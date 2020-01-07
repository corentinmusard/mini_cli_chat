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
 * Add current time in TIME_FORMAT format to `buffer`
 * Return an malloc-ed char*
 * Returned value is current time in TIME_FORMAT format followed by `buffer` of size `size`
 */
char* log_format(const char *buffer, size_t size);

#ifdef __cplusplus
}
#endif
#endif
