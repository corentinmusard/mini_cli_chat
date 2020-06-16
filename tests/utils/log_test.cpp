#include <assert.h>
#include <gtest/gtest.h>

#include "log.h"
#include "tests/utils.hpp"

/**
 * define setter to modify the return value of the fake time function
 */
static time_t t = 0;
static void set_time(time_t new_t) {
    t = new_t;
}

time_t time(time_t *tloc __attribute__ ((unused))) {
    return t;
}

class LogTest : public ::testing::Test
{
protected:
    void SetUp() override {
        setenv("TZ", "UTC", 1);
        set_time(0);
        logfile = tmpnam(NULL);
        set_logfile(logfile);
    }

    void TearDown() override {
        free(buffer);
    }

    char *buffer = NULL;
    char *logfile = NULL;
};

TEST_F(LogTest, info_empty_string)
{
    info("");

    read_equal_name(logfile, "00:00:00 ");
}

TEST_F(LogTest, info_constant_string)
{
    info("Hello you!\n");

    read_equal_name(logfile, "00:00:00 Hello you!\n");
}

TEST_F(LogTest, info_complex_string)
{
    info("Hello %s 4*5=%d", "Bob", 20);

    read_equal_name(logfile, "00:00:00 Hello Bob 4*5=20");
}

TEST_F(LogTest, info_non_null_time_less_than_one_day)
{
    set_time(2*3600 + 3*60 + 45);

    info("Hello you!");

    read_equal_name(logfile, "02:03:45 Hello you!");
}

TEST_F(LogTest, info_non_null_time_greater_than_one_day)
{
    set_time(18*24*3600 + 2*3600 + 3*60 + 45);

    info("Hello you!");

    read_equal_name(logfile, "02:03:45 Hello you!");
}

TEST_F(LogTest, log_ormat_empty_string)
{
    const char str[] = "";
    buffer = log_format(str, sizeof(str));

    ASSERT_STREQ(buffer, "00:00:00 ");
}

TEST_F(LogTest, log_ormat_constant_string)
{
    const char str[] = "Hello you!\n";
    buffer = log_format(str, sizeof(str));

    ASSERT_STREQ(buffer, "00:00:00 Hello you!\n");
}

TEST_F(LogTest, log_ormat_non_null_time_less_than_one_day)
{
    const char str[] = "Hello you!";
    set_time(2*3600 + 3*60 + 45);
    buffer = log_format(str, sizeof(str));

    ASSERT_STREQ(buffer, "02:03:45 Hello you!");
}

TEST_F(LogTest, log_ormat_non_null_time_greater_than_one_day)
{
    const char str[] = "Hello you!";
    set_time(18*24*3600 + 2*3600 + 3*60 + 45);
    buffer = log_format(str, sizeof(str));

    ASSERT_STREQ(buffer, "02:03:45 Hello you!");
}

TEST_F(LogTest, log_ormat_truncate)
{
    const char str[] = "Hello you!";
    buffer = log_format(str, 4);

    ASSERT_STREQ(buffer, "00:00:00 Hello");
    read_equal_name(logfile, "00:00:00 log_format: truncated output: len=19, m_size=15\n");
}

class LogInitTest : public ::testing::Test
{
protected:
    void SetUp() override {
        setenv("TZ", "UTC", 1);
        set_time(0);
        unset_logfile();
    }

    void TearDown() override {
    }

    #define STDERR_TO_BUFFER(f) do {\
        FILE *e = freopen("/dev/null", "a", stderr);\
        assert(e);\
        setbuf(stderr, buffer);\
        f;\
        e = freopen("/dev/tty", "a", stderr);\
        assert(e);\
    } while (0)

    char buffer[BUFSIZ] = {0};
};

TEST_F(LogInitTest, log_init_null)
{
    set_logfile(NULL);

    STDERR_TO_BUFFER(info("hello"));

    ASSERT_STREQ(buffer, "00:00:00 hello");
}

TEST_F(LogInitTest, log_init)
{
    char *logfile = tmpnam(NULL);
    set_logfile(logfile);

    STDERR_TO_BUFFER(info("hello"));

    ASSERT_STREQ(buffer, "");
    read_equal_name(logfile, "00:00:00 hello");
}
