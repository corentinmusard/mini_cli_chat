#ifndef SERVER_SERVER_SETTINGS_H
#define SERVER_SERVER_SETTINGS_H
#ifdef __cplusplus
extern "C" {
#endif

#include <netinet/in.h>

/**
 * Max logfile length
 */
#define MAX_LOGFILE_LENGTH 20

typedef struct {
    in_port_t port; // port number to run the server at
    char log_file[MAX_LOGFILE_LENGTH + 1]; // log file to write debug info
} Settings;

/**
 * Return default settings
 */
Settings default_settings(void);

/**
 * Intialize settings from `settings_file`
 * if `settings_file` is NULL, return default settings
 * Return a struct containing all the settings
 */
Settings settings_init(const char *settings_file);

#ifdef __cplusplus
}
#endif
#endif
