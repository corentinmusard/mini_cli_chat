#include <curses.h>
#include <gtest/gtest.h>

/**
 * define getter/setter to modify the return value of the fake getmaxyx macro
 */
static int _max_x;
static void set_max_x(int new_x) {
    _max_x = new_x;
}
static int get_max_x(void) {
    return _max_x;
}

#undef getmaxyx
#define getmaxyx(win,y,_x) do {\
    _x = _max_x;\
} while (0)

#include "client/client_lib.c"

class ClientLibTest : public ::testing::Test
{
protected:
    void SetUp() override {
        input = input_init();
        set_max_x(100);
    }

    void TearDown() override {
        free_input(input);
    }

    Input *input;
};

TEST_F(ClientLibTest, increment_indice_message)
{
    increment_indice_message(input);
    ASSERT_EQ(input->i, 1);
}

TEST_F(ClientLibTest, increment_indice_message_max_minus_one)
{
    int i = get_max_x() - INITIAL_MESSAGE_X - 1;
    input->i = i;

    increment_indice_message(input);
    ASSERT_EQ(input->i, i + 1);
}

TEST_F(ClientLibTest, increment_indice_message_equal_max)
{
    int i = get_max_x() - INITIAL_MESSAGE_X;
    input->i = i;

    increment_indice_message(input);
    ASSERT_EQ(input->i, i);
}

TEST_F(ClientLibTest, increment_indice_message_superior_max)
{
    int i = get_max_x() + 1000;
    input->i = i;

    increment_indice_message(input);
    ASSERT_EQ(input->i, get_max_x() - INITIAL_MESSAGE_X);
}

TEST_F(ClientLibTest, reset_variables)
{
    input->buffer[0] = 'a';
    input->buffer[1] = 'b';
    input->buffer[2] = 'c';
    input->buffer[3] = 'd';
    input->i = 4;

    reset_variables(input);

    ASSERT_EQ(strlen(input->buffer), 0);
    ASSERT_EQ(input->i, 0);
}
