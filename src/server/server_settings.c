#include <assert.h>
#include <cJSON/cJSON.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "server_settings.h"
#include "utils.h"

/**
 * Default log file
 */
#define DEFAULT_LOG_FILE "server.log"

Settings default_settings(void) {
    Settings s = {
        .port = PORT,
        .log_file = DEFAULT_LOG_FILE,
    };
    return s;
}

/**
 * Update port field in `s` from `json` values
 */
static void update_port(Settings *s, const cJSON *json) {
    const cJSON *port = cJSON_GetObjectItemCaseSensitive(json, "port");
    if (port == NULL) {
        return;
    }

    if (!cJSON_IsNumber(port)) {
        info("port: field should be an int\n");
        return;
    }

    if (port->valueint < 0 || port->valueint > USHRT_MAX) {
        info("port: invalid value (should be 0<=port<=%d): %d\n",
            USHRT_MAX,
            port->valueint
        );
        return;
    }

    s->port = (in_port_t)port->valueint;
    info("port: taking value: %d\n", s->port);
}

/**
 * Update log_file field in `s` from `json` values
 */
static void update_logfile(Settings *s, const cJSON *json) {
    const cJSON *log_file = cJSON_GetObjectItemCaseSensitive(json, "log_file");
    if (log_file == NULL) {
        return;
    }

    if (!cJSON_IsString(log_file)) {
        info("log_file: field should be a string\n");
        return;
    }

    size_t len = strlen(log_file->valuestring);
    if (len == 0) {
        info("log_file: field must not be empty\n");
        return;
    }

    if (len > MAX_LOGFILE_LENGTH) {
        info("log_file: file name too long (max: %d, found: %d): %d\n",
            MAX_LOGFILE_LENGTH,
            len,
            log_file->valuestring
        );
        return;
    }

    strncpy(s->log_file, log_file->valuestring, MAX_LOGFILE_LENGTH);
    info("log_file: taking value: %s\n", s->log_file);
}

/**
 * Parse json from `buffer`
 * Update `s` accordingly
 */
static void parse_json(Settings *s, const char *buffer) {
    assert(s && "should not be NULL");
    assert(buffer && "should not be NULL");
    assert(buffer[0] && "should be '{'");

    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL) {
        info("cJSON_Parse: Error before: %s\n", cJSON_GetErrorPtr());
        return;
    }

    update_port(s, json);    
    update_logfile(s, json);

    cJSON_Delete(json);
}

/**
 * Return the file size
 */
static size_t get_file_length(FILE *f) {
    assert(f && "should not be NULL");

    fseek(f, 0, SEEK_END);
    long l = ftell(f);
    if (l == -1) {
        perror("ftell");
        exit(EXIT_FAILURE);
    }
    return (size_t)l;
}

/**
 * Return a malloc-ed buffer containing the content of `f` of length l
 */
static char* file_to_buffer(FILE *f, size_t l) {
    assert(f && "should not be NULL");

    char *buffer = malloc(l + 1);
    assert(buffer);

    size_t n = fread(buffer, 1, l, f);
    if (n != l) {
        info("problem n=%zu\n", n);
        exit(EXIT_FAILURE);
    }
    buffer[l] = '\0';
    return buffer;
}

Settings settings_init(const char *settings_file) {
    Settings s = default_settings();

    if (settings_file == NULL) {
        return s;
    }

    FILE *f = fopen(settings_file, "r");
    if (f == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    size_t length = get_file_length(f);
    if (length == 0) {
        fclose(f);
        return s;
    }

    rewind(f);
    char *buffer = file_to_buffer(f, length);
    fclose(f);

    parse_json(&s, buffer);

    free(buffer);
    return s;
}
