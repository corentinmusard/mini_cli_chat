#include <gtest/gtest.h>

#include "client/client_settings.h"
#include "tests/utils.hpp"

class ClientSettingsTest : public ::testing::Test
{
protected:
    void SetUp() override {
        s = client_default_settings();
    }

    void TearDown() override {
    }

    void assert_default_settings() {
        Client_settings default_s = client_default_settings();
        EXPECT_EQ(s.port, default_s.port);
        EXPECT_STREQ(s.log_file, default_s.log_file);
    }

    Client_settings s;
};

TEST_F(ClientSettingsTest, settings_init_null)
{
    client_settings_init(NULL);

    assert_default_settings();
}

TEST_F(ClientSettingsTest, settings_init_empty)
{
    const char buffer[] = "{}";
    char *settings_file = fill_tmp_file(buffer);

    s = client_settings_init(settings_file);

    assert_default_settings();
}

TEST_F(ClientSettingsTest, settings_init_ip)
{
    const char buffer[] = "{\"ip\": \"1.2.3.4\"}";
    char *settings_file = fill_tmp_file(buffer);

    s = client_settings_init(settings_file);

    EXPECT_STREQ(s.ip, "1.2.3.4");
}

TEST_F(ClientSettingsTest, settings_init_ip_wrong_type)
{
    const char buffer[] = "{\"ip\": 5}";
    char *settings_file = fill_tmp_file(buffer);

    s = client_settings_init(settings_file);

    assert_default_settings();
}

TEST_F(ClientSettingsTest, settings_init_ip_ipv6_not_supported)
{
    const char buffer[] = "{\"ip\": \"::1\"}";
    char *settings_file = fill_tmp_file(buffer);

    s = client_settings_init(settings_file);

    assert_default_settings();
}

TEST_F(ClientSettingsTest, settings_init_ip_bad_value)
{
    const char buffer[] = "{\"ip\": \"1.2.3.300\"}";
    char *settings_file = fill_tmp_file(buffer);

    s = client_settings_init(settings_file);

    assert_default_settings();
}

TEST_F(ClientSettingsTest, settings_init_nickname)
{
    const char buffer[] = "{\"nickname\": \"corentin\"}";
    char *settings_file = fill_tmp_file(buffer);

    s = client_settings_init(settings_file);

    EXPECT_STREQ(s.nickname, "corentin");
}

TEST_F(ClientSettingsTest, settings_init_nickname_wrong_type)
{
    const char buffer[] = "{\"nickname\": 5}";
    char *settings_file = fill_tmp_file(buffer);

    s = client_settings_init(settings_file);

    assert_default_settings();
}

TEST_F(ClientSettingsTest, settings_init_nickname_too_long)
{
    //todo: make file length using MAX_USERNAME_LENGTH
    const char buffer[] = "{\"nickname\": \"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\"}";
    char *settings_file = fill_tmp_file(buffer);

    s = client_settings_init(settings_file);

    assert_default_settings();
}
