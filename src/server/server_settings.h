#ifndef SERVER_SERVER_SETTINGS_H
#define SERVER_SERVER_SETTINGS_H
#ifdef __cplusplus
extern "C" {
#endif

#include "utils/settings.h"

typedef struct {
    BASE_SETTINGS; //default settings fields (like port or log_file)

    // define more values below
} Server_settings;

/**
 * Return default settings
 */
Server_settings server_default_settings(void);

/**
 * Register specific server settings
 * Return settings from `settings_file` if not NULL
 */
Server_settings server_settings_init(const char *settings_file);

#ifdef __cplusplus
}
#endif
#endif
