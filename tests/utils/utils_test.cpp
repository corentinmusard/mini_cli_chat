#include <gtest/gtest.h>
#include <signal.h>

#include "utils/utils.h"

class UtilsTest : public ::testing::Test
{
protected:
    void SetUp() override {
        interrupt = 0;
        err = register_sigint();
    }

    int err;
};

TEST_F(UtilsTest, register_sigint)
{
    EXPECT_EQ(err, 0);
}

TEST_F(UtilsTest, exit_not_wanted)
{
    EXPECT_TRUE(exit_not_wanted(0));
}

TEST_F(UtilsTest, exit_not_wanted_err)
{
    EXPECT_FALSE(exit_not_wanted(-1));
}

TEST_F(UtilsTest, exit_not_wanted_interrupt)
{
    int e = raise(SIGINT);
    EXPECT_EQ(e, 0);
    EXPECT_FALSE(exit_not_wanted(0));
}

TEST_F(UtilsTest, exit_not_wanted_err_interrupt)
{
    int e = raise(SIGINT);
    EXPECT_EQ(e, 0);
    EXPECT_FALSE(exit_not_wanted(-1));
}

TEST_F(UtilsTest, want_to_exit)
{
    EXPECT_TRUE(exit_not_wanted(0));
    want_to_exit();
    EXPECT_FALSE(exit_not_wanted(0));
}

TEST(is_stdin, False)
{
    EXPECT_FALSE(is_stdin(STDIN_FILENO + 1));
}
