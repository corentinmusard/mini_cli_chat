#include <gtest/gtest.h>
#include <signal.h>

#include "utils.h"

class UtilsTest : public ::testing::Test
{
protected:
    void SetUp() override {
        err = register_sigint();
    }

    int err;
};

TEST_F(UtilsTest, register_sigint)
{
    ASSERT_EQ(err, 0);
}

TEST_F(UtilsTest, exit_not_wanted)
{
    ASSERT_TRUE(exit_not_wanted(0));
}

TEST_F(UtilsTest, exit_not_wanted_err)
{
    ASSERT_FALSE(exit_not_wanted(-1));
}

TEST_F(UtilsTest, exit_not_wanted_interrupt)
{
    int e = raise(SIGINT);
    ASSERT_EQ(e, 0);
    ASSERT_FALSE(exit_not_wanted(0));
}

TEST_F(UtilsTest, exit_not_wanted_err_interrupt)
{
    int e = raise(SIGINT);
    ASSERT_EQ(e, 0);
    ASSERT_FALSE(exit_not_wanted(-1));
}

TEST(is_stdin, True)
{
    ASSERT_TRUE(is_stdin(STDIN_FILENO));
}

TEST(is_stdin, False)
{
    ASSERT_FALSE(is_stdin(STDIN_FILENO + 1));
}
