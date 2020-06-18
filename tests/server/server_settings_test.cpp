#include <gtest/gtest.h>

#include "server/server_settings.h"
#include "tests/utils.hpp"

class ServerSettingsTest : public ::testing::Test
{
protected:
    void SetUp() override {
        s = server_default_settings();
    }

    void TearDown() override {
    }

    void assert_default_settings() {
        Server_settings default_s = server_default_settings();
        ASSERT_EQ(s.port, default_s.port);
        ASSERT_STREQ(s.log_file, default_s.log_file);
    }

    Server_settings s;
};

TEST_F(ServerSettingsTest, settings_init_null)
{
    s = server_settings_init(NULL);

    assert_default_settings();
}

TEST_F(ServerSettingsTest, settings_init_empty)
{
    const char buffer[] = "{}";
    char *settings_file = fill_tmp_file(buffer);

    s = server_settings_init(settings_file);

    assert_default_settings();
}