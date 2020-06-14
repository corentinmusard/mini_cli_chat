#include <gtest/gtest.h>

#include "tests/fake_functions.hpp"

#include "client/cli.c"

class CliTest : public ::testing::Test
{
protected:
    void SetUp() override {
        init_cli(&window_msgs, &window_input);

        // resets fake functions
        RESET_FAKE(doupdate)
        RESET_FAKE(wclrtoeol)
        RESET_FAKE(wmove)
        RESET_FAKE(wnoutrefresh)
        // reset common FFF internal structures
        FFF_RESET_HISTORY()
    }

    void TearDown() override {
        endwin();
    }

    WINDOW *window_msgs;
    WINDOW *window_input;
};

TEST_F(CliTest, init_cli)
{
    ASSERT_NE(window_msgs, nullptr);
    ASSERT_NE(window_input, nullptr);
}

TEST_F(CliTest, reset_message_cursor)
{
    reset_message_cursor(window_msgs);
    ASSERT_EQ(wmove_fake.call_count, 1);
    ASSERT_EQ(wmove_fake.arg0_history[0], window_msgs);
    ASSERT_EQ(wmove_fake.arg1_history[0], INITIAL_MESSAGE_Y);
    ASSERT_EQ(wmove_fake.arg2_history[0], INITIAL_MESSAGE_X);
}

TEST_F(CliTest, refresh_cli)
{
    refresh_cli(window_msgs, window_input);
    ASSERT_EQ(wnoutrefresh_fake.call_count, 2);
    ASSERT_EQ(doupdate_fake.call_count, 1);
}

TEST_F(CliTest, clear_message_area)
{
    clear_message_area(window_msgs);
    ASSERT_EQ(wclrtoeol_fake.call_count, 1);
}
