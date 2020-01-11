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

class LogTest : public ::testing::Test
{
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

TEST_F(LogTest, info)
{
    STDOUT_TO_BUFFER(info(NULL));

    ASSERT_STREQ(buffer, "00:00:00 ");
}

TEST_F(LogTest, info_empty_string)
{
    STDOUT_TO_BUFFER(info(""));

    ASSERT_STREQ(buffer, "00:00:00 ");
}

TEST_F(LogTest, info_constant_string)
{
    STDOUT_TO_BUFFER(info("Hello you!\n"));

    ASSERT_STREQ(buffer, "00:00:00 Hello you!\n");
}

TEST_F(LogTest, info_complex_string)
{
    STDOUT_TO_BUFFER(info("Hello %s 4*5=%d", "Bob", 20));

    ASSERT_STREQ(buffer, "00:00:00 Hello Bob 4*5=20");
}

TEST_F(LogTest, info_non_null_time_less_than_one_day)
{
    set_time(2*3600 + 3*60 + 45);

    STDOUT_TO_BUFFER(info("Hello you!"));

    ASSERT_STREQ(buffer, "02:03:45 Hello you!");
}

TEST_F(LogTest, info_non_null_time_greater_than_one_day)
{
    set_time(18*24*3600 + 2*3600 + 3*60 + 45);

    STDOUT_TO_BUFFER(info("Hello you!"));

    ASSERT_STREQ(buffer, "02:03:45 Hello you!");
}
