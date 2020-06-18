#include <assert.h>
#include <cJSON/cJSON.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "server_settings.h"
#include "utils.h"

Server_settings server_default_settings(void) {
    Server_settings s = {
        .port = PORT,
        .log_file = "server.log",
    };
    return s;
}

Server_settings server_settings_init(const char *settings_file) {
    Server_settings s = server_default_settings();
    settings_init(settings_file, (Settings *)&s);

    return s;
}
