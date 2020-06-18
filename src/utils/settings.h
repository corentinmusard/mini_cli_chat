#ifndef UTILS_SETTINGS_H
#define UTILS_SETTINGS_H
#ifdef __cplusplus
extern "C" {
#endif

#include <cJSON/cJSON.h>
#include <netinet/in.h>

/**
 * Max logfile length
 */
#define MAX_LOGFILE_LENGTH 20

/**
 * port: port number to run the server at
 * log_file: log file to write debug info
 */
#define BASE_SETTINGS \
    struct {\
        in_port_t port;\
        char log_file[MAX_LOGFILE_LENGTH + 1];\
    }

typedef BASE_SETTINGS Settings;

/**
 * Return default settings
 */
Settings default_settings(void);

typedef void (*Update)(Settings *s, const cJSON *json);

/**
 * Register the function `update` to read from `json` and modify `s`.
 * It will be called when the `settings_init` function is called
 */
void add_setting(Update update);

/**
 * Intialize settings from `settings_file`
 * if `settings_file` is NULL, do nothing
 * Update the struct `s` with all the updated settings
 */
void settings_init(const char *settings_file, Settings *s);

#ifdef __cplusplus
}
#endif
#endif
