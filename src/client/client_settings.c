#include <arpa/inet.h>
#include <assert.h>
#include <cJSON/cJSON.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#include "utils/log.h"
#include "utils/utils.h"

#include "client_settings.h"

Client_settings client_default_settings(void) {
    Client_settings s = {
        .port = PORT,
        .log_file = "client.log",
        .ip = "127.0.0.1",
        .nickname = "user"
    };
    return s;
}

/**
 * Update ip field in `s` from `json` values
 */
static void update_ip(Settings *s, const cJSON *json) {
    assert(s && "should not be NULL");
    assert(json && "should not be NULL");

    const cJSON *ip = cJSON_GetObjectItemCaseSensitive(json, "ip");
    if (ip == NULL) {
        return;
    }

    if (!cJSON_IsString(ip)) {
        info("ip: field should be a string\n");
        return;
    }

    struct in6_addr addr6;
    if (inet_pton(AF_INET6, ip->valuestring, &addr6) == 1) {
        info("ip: ipv6 is not supported: %s\n", ip->valuestring);
        return;
    }

    struct in_addr addr;
    if (inet_pton(AF_INET, ip->valuestring, &addr) != 1) {
        info("ip: invalid value: %s\n", ip->valuestring);
        return;
    }

    Client_settings *s2 = (Client_settings *)s;
    strncpy(s2->ip, ip->valuestring, MAX_IP_LENGTH);
    info("ip: taking value: %s\n", s2->ip);
}

/**
 * Update nickname field in `s` from `json` values
 */
static void update_nickname(Settings *s, const cJSON *json) {
    assert(s && "should not be NULL");
    assert(json && "should not be NULL");

    const cJSON *nickname = cJSON_GetObjectItemCaseSensitive(json, "nickname");
    if (nickname == NULL) {
        return;
    }

    if (!cJSON_IsString(nickname)) {
        info("nickname: field should be a string\n");
        return;
    }

    size_t len = strlen(nickname->valuestring);
    if (len > MAX_USERNAME_LENGTH) {
        info("nickname: nickname too long (max: %d, found: %d) : %s",
            MAX_USERNAME_LENGTH,
            len,
            nickname->valuestring
        );
        return;
    }

    Client_settings *s2 = (Client_settings *)s;
    strncpy(s2->nickname, nickname->valuestring, MAX_USERNAME_LENGTH);
    info("nickname: taking value: %s\n", s2->nickname);
}

Client_settings client_settings_init(const char *settings_file) {
    add_setting(update_ip);
    add_setting(update_nickname);

    Client_settings s = client_default_settings();
    settings_init(settings_file, (Settings *)&s);

    return s;
}
