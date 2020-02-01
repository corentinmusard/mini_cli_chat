#include <gtest/gtest.h>

#include "client/client_lib.h"

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

