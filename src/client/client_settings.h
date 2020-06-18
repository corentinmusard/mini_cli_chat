#ifndef CLIENT_CLIENT_SETTINGS_H
#define CLIENT_CLIENT_SETTINGS_H
#ifdef __cplusplus
extern "C" {
#endif

#include <netinet/in.h>

#include "settings.h"
#include "utils.h"

/**
 * See https://stackoverflow.com/a/1076755
 */
#define MAX_IP_LENGTH 45

typedef struct {
    BASE_SETTINGS; //default settings fields (like port or log_file)

    // define more values below
    char ip[MAX_IP_LENGTH + 1];
    char nickname[MAX_USERNAME_LENGTH + 1];
} Client_settings;

/**
 * Return default settings
 */
Client_settings client_default_settings(void);

/**
 * Register specific client settings
 * Return settings from `settings_file` if not NULL
 */
Client_settings client_settings_init(const char *settings_file);

#ifdef __cplusplus
}
#endif
#endif
