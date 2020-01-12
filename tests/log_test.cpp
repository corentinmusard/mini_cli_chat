#include <gtest/gtest.h>
#include <unistd.h>

#include "../src/utils/log.h"

static time_t t = 0;

time_t time(time_t *tloc) {
    return t;
}

void set_time(time_t new_t) {
    t = new_t;
}

class InfoTest : public ::testing::Test
{
protected:
    void SetUp() override {
        setenv("TZ", "UTC", 1);
        set_time(0);
    }

    #define STDOUT_TO_BUFFER(f) do {\
        freopen("/dev/null", "a", stdout);\
        setbuf(stdout, buffer);\
        f;\
        freopen("/dev/tty", "a", stdout);\
    } while (0)

public:
    char buffer[BUFSIZ] = {0};
};

TEST_F(InfoTest, info)
{
    STDOUT_TO_BUFFER(info(NULL));

    ASSERT_STREQ(buffer, "00:00:00 ");
}

TEST_F(InfoTest, info_empty_string)
{
    STDOUT_TO_BUFFER(info(""));

    ASSERT_STREQ(buffer, "00:00:00 ");
}

TEST_F(InfoTest, info_constant_string)
{
    STDOUT_TO_BUFFER(info("Hello you!\n"));

    ASSERT_STREQ(buffer, "00:00:00 Hello you!\n");
}

TEST_F(InfoTest, info_complex_string)
{
    STDOUT_TO_BUFFER(info("Hello %s 4*5=%d", "Bob", 20));

    ASSERT_STREQ(buffer, "00:00:00 Hello Bob 4*5=20");
}

TEST_F(InfoTest, info_non_null_time_less_than_one_day)
{
    set_time(2*3600 + 3*60 + 45);

    STDOUT_TO_BUFFER(info("Hello you!"));

    ASSERT_STREQ(buffer, "02:03:45 Hello you!");
}

TEST_F(InfoTest, info_non_null_time_greater_than_one_day)
{
    set_time(18*24*3600 + 2*3600 + 3*60 + 45);

    STDOUT_TO_BUFFER(info("Hello you!"));

    ASSERT_STREQ(buffer, "02:03:45 Hello you!");
}

class LogFormatTest : public ::testing::Test
{
protected:
    void SetUp() override {
        setenv("TZ", "UTC", 1);
        set_time(0);
    }

    void TearDown() override {
        free(buffer);
    }

public:
    char *buffer = NULL;
};

TEST_F(LogFormatTest, log_format)
{
    const char *str = NULL;
    buffer = log_format(str, sizeof(str));

    ASSERT_STREQ(buffer, "00:00:00 ");
}

TEST_F(LogFormatTest, log_format_empty_string)
{
    const char str[] = "";
    buffer = log_format(str, sizeof(str));

    ASSERT_STREQ(buffer, "00:00:00 ");
}

TEST_F(LogFormatTest, log_format_constant_string)
{
    const char str[] = "Hello you!\n";
    buffer = log_format(str, sizeof(str));

    ASSERT_STREQ(buffer, "00:00:00 Hello you!\n");
}

TEST_F(LogFormatTest, log_format_non_null_time_less_than_one_day)
{
    const char str[] = "Hello you!";
    set_time(2*3600 + 3*60 + 45);
    buffer = log_format(str, sizeof(str));

    ASSERT_STREQ(buffer, "02:03:45 Hello you!");
}

TEST_F(LogFormatTest, log_format_non_null_time_greater_than_one_day)
{
    const char str[] = "Hello you!";
    set_time(18*24*3600 + 2*3600 + 3*60 + 45);
    buffer = log_format(str, sizeof(str));

    ASSERT_STREQ(buffer, "02:03:45 Hello you!");
}