#include <gtest/gtest.h>
#include <signal.h>

#include "utils/settings.h"
#include "tests/utils.hpp"

class SettingsTest : public ::testing::Test
{
protected:
    void SetUp() override {
        s = default_settings();
    }

    void assert_default_settings() {
        Settings default_s = default_settings();
        EXPECT_EQ(s.port, default_s.port);
        EXPECT_STREQ(s.log_file, default_s.log_file);
    }

    Settings s;
};

TEST_F(SettingsTest, settings_init_null)
{
    settings_init(NULL, &s);

    assert_default_settings();
}

TEST_F(SettingsTest, settings_init_empty_file)
{
    const char buffer[1] = "";
    char *settings_file = fill_tmp_file(buffer);

    settings_init(settings_file, &s);

    assert_default_settings();
}

TEST_F(SettingsTest, settings_init_file_wrong_format)
{
    const char buffer[6] = ".();{";
    char *settings_file = fill_tmp_file(buffer);

    settings_init(settings_file, &s);

    assert_default_settings();
}

TEST_F(SettingsTest, settings_init_file)
{
    const char buffer[3] = "{}";
    char *settings_file = fill_tmp_file(buffer);

    settings_init(settings_file, &s);

    assert_default_settings();
}

TEST_F(SettingsTest, settings_init_port)
{
    const char buffer[] = "{\"port\": 42}";
    char *settings_file = fill_tmp_file(buffer);

    settings_init(settings_file, &s);

    EXPECT_EQ(s.port, 42);
}

TEST_F(SettingsTest, settings_init_port_bad_type)
{
    const char buffer[] = "{\"port\": \"47\"}";
    char *settings_file = fill_tmp_file(buffer);

    settings_init(settings_file, &s);

    assert_default_settings();
}

TEST_F(SettingsTest, settings_init_bad_port)
{
    const char buffer[] = "{\"port\": 7878787}";
    char *settings_file = fill_tmp_file(buffer);

    settings_init(settings_file, &s);

    assert_default_settings();
}

TEST_F(SettingsTest, settings_init_bad_port2)
{
    const char buffer[] = "{\"port\": -5}";
    char *settings_file = fill_tmp_file(buffer);

    settings_init(settings_file, &s);

    assert_default_settings();
}

TEST_F(SettingsTest, settings_init_log_file)
{
    const char buffer[] = "{\"log_file\": \"myserver.log\"}";
    char *settings_file = fill_tmp_file(buffer);

    settings_init(settings_file, &s);

    EXPECT_STREQ(s.log_file, "myserver.log");
}

TEST_F(SettingsTest, settings_init_log_file_bad_type)
{
    const char buffer[] = "{\"log_file\": 3}";
    char *settings_file = fill_tmp_file(buffer);

    settings_init(settings_file, &s);

    assert_default_settings();
}

TEST_F(SettingsTest, settings_init_log_file_empty_field)
{
    const char buffer[] = "{\"log_file\": \"\"}";
    char *settings_file = fill_tmp_file(buffer);

    settings_init(settings_file, &s);

    assert_default_settings();
}

TEST_F(SettingsTest, settings_init_log_file_too_long)
{
    // todo: make file length using MAX_LOGFILE_LENGTH
    const char buffer[] = "{\"log_file\": \"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA.log\"}";
    char *settings_file = fill_tmp_file(buffer);

    settings_init(settings_file, &s);

    assert_default_settings();
}
