#include <gtest/gtest.h>

#include "tests/fake_functions.hpp"

#include "client/screen.h"

class InputTest : public ::testing::Test
{
protected:
    void SetUp() override {
        input = input_init();
    }

    void TearDown() override {
        free_input(input);
    }

    Input *input;
};

TEST_F(InputTest, input_init)
{
    ASSERT_EQ(input->i, 0);
}


class MessagesTest : public ::testing::Test
{
protected:
    void SetUp() override {
        messages = messages_init();
    }

    void TearDown() override {
        free_messages(messages);
    }

    Messages *messages;
};

TEST_F(MessagesTest, messages_init)
{
    ASSERT_EQ(messages->y, 0);
}

class ScreenTest : public ::testing::Test
{
protected:
    void SetUp() override {
        screen = screen_init();

        // resets fake functions
        RESET_FAKE(doupdate)
        RESET_FAKE(wnoutrefresh)
        // reset common FFF internal structures
        FFF_RESET_HISTORY()
    }

    void TearDown() override {
        free_screen(screen);
    }

    Screen *screen;
};

TEST_F(ScreenTest, screen_init)
{
    //tested code are in SetUp and TearDown
    SUCCEED();
}

TEST_F(ScreenTest, refresh_screen)
{
    refresh_screen(screen);
    ASSERT_EQ(wnoutrefresh_fake.call_count, 2);
    ASSERT_EQ(doupdate_fake.call_count, 1);
}
