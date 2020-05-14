#include <curses.h>
#include <fff/fff.h>
#include <gtest/gtest.h>

#include "tests/utils.hpp"

//create fake functions
DEFINE_FFF_GLOBALS
FAKE_VALUE_FUNC_VARARG(int, mvwprintw, WINDOW *, int, int, const char *, ...)
FAKE_VALUE_FUNC(int, wmove, WINDOW *, int, int)
FAKE_VALUE_FUNC(int, wdelch, WINDOW *)

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
        screen = screen_init();
        input = screen->input;
        msgs = screen->msgs;
        set_max_x(100);

        fake_fd = get_fake_fd();
        interrupt = 0;

        // resets fake functions
        RESET_FAKE(mvwprintw);
        RESET_FAKE(wdelch);
        RESET_FAKE(wmove);
        // reset common FFF internal structures
        FFF_RESET_HISTORY();
    }

    void TearDown() override {
        free_screen(screen);
    }

    Input *input;
    Messages* msgs;
    Screen *screen;
    int fake_fd;
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

    ASSERT_STREQ(input->buffer, "");
    ASSERT_EQ(input->i, 0);
}

TEST_F(ClientLibTest, print_message)
{
    print_message(msgs, "helloo");
    ASSERT_EQ(mvwprintw_fake.call_count, 1);
    ASSERT_STREQ(mvwprintw_fake.arg3_history[0], "helloo");
}

TEST_F(ClientLibTest, print_message_inc_y)
{
    int old_y = msgs->y;
    print_message(msgs, "helloo");
    ASSERT_EQ(msgs->y, old_y + 1);
}

TEST_F(ClientLibTest, print_input_char)
{
    print_input_char(input, 'b');
    // fff don't enable uses of arg4_history to check if it equals 'b'
    ASSERT_EQ(mvwprintw_fake.call_count, 1);
    ASSERT_STREQ(mvwprintw_fake.arg3_history[0], "%c");
}

TEST_F(ClientLibTest, delete_message_character_remove_char_buffer)
{
    input->i = 3;
    input->buffer[0] =  'A';
    input->buffer[1] =  'B';
    input->buffer[2] =  'C';
    input->buffer[3] =  'D';
    delete_message_character(input);
    ASSERT_STREQ(input->buffer, "ABC");
}

TEST_F(ClientLibTest, delete_message_character_remove_char_window)
{
    input->i = 3;
    input->buffer[0] =  'A';
    input->buffer[1] =  'B';
    input->buffer[2] =  'C';
    input->buffer[3] =  'D';
    delete_message_character(input);
    ASSERT_EQ(wdelch_fake.call_count, 1);
    ASSERT_EQ(wdelch_fake.arg0_history[0], input->window);
}

TEST_F(ClientLibTest, delete_message_character_update_i)
{
    input->i = 3;
    input->buffer[0] =  'A';
    input->buffer[1] =  'B';
    input->buffer[2] =  'C';
    input->buffer[3] =  'D';
    delete_message_character(input);
    ASSERT_EQ(input->i, 2);
}

TEST_F(ClientLibTest, delete_message_character_update_i_zero)
{
    input->i = 0;
    delete_message_character(input);
    ASSERT_EQ(input->i, 0);
}

TEST_F(ClientLibTest, delete_message_character_move_cursor)
{
    input->i = 3;
    input->buffer[0] =  'A';
    input->buffer[1] =  'B';
    input->buffer[2] =  'C';
    input->buffer[3] =  'D';
    delete_message_character(input);
    ASSERT_EQ(wmove_fake.call_count, 1);
    ASSERT_EQ(wmove_fake.arg2_history[0], INITIAL_MESSAGE_X + 3 - 1);
}

TEST_F(ClientLibTest, input_char_handling_update_i)
{
    input_char_handling(input, 'A');
    ASSERT_EQ(input->i, 1);
}

TEST_F(ClientLibTest, input_char_handling_store_c)
{
    input_char_handling(input, 'A');
    ASSERT_EQ(input->buffer[0], 'A');
}

TEST_F(ClientLibTest, input_char_handling_print_c)
{
    input_char_handling(input, 'A');
    ASSERT_EQ(mvwprintw_fake.call_count, 1);
}

TEST_F(ClientLibTest, execute_command)
{
    int e = execute_command("", fake_fd);
    ASSERT_EQ(e, UNKNOWN_COMMAND);
}

TEST_F(ClientLibTest, execute_command_quit)
{
    ASSERT_TRUE(exit_not_wanted(1));
    int e = execute_command("/quit", fake_fd);
    ASSERT_EQ(e, 0);
    ASSERT_FALSE(exit_not_wanted(1));
}

TEST_F(ClientLibTest, execute_command_q)
{
    ASSERT_TRUE(exit_not_wanted(1));
    int e = execute_command("/q", fake_fd);
    ASSERT_EQ(e, 0);
    ASSERT_FALSE(exit_not_wanted(1));
}

TEST_F(ClientLibTest, execute_command_nick)
{
    int e = execute_command("/nick my_nick", fake_fd);
    ASSERT_EQ(e, 0);
    read_equal(fake_fd, "/nick my_nick");
}

TEST_F(ClientLibTest, execute_command_nick_bad_fd)
{
    int e = execute_command("/nick my_nick", BAD_FD);
    ASSERT_EQ(e, -1);
}

TEST_F(ClientLibTest, execute_command_nick_empty)
{
    int e = execute_command("/nick", fake_fd);
    ASSERT_EQ(e, UNKNOWN_COMMAND);
}

TEST_F(ClientLibTest, execute_command_send)
{
    int e = execute_command("/send my msg :)", fake_fd);
    ASSERT_EQ(e, 0);
    read_equal(fake_fd, "my msg :)");
}

TEST_F(ClientLibTest, execute_command_send_bad_fd)
{
    int e = execute_command("/send my msg :)", BAD_FD);
    ASSERT_EQ(e, -1);
}

TEST_F(ClientLibTest, execute_command_send_empty)
{
    int e = execute_command("/send ", fake_fd);
    ASSERT_EQ(e, 0);
}

TEST_F(ClientLibTest, display_message)
{
    display_message(msgs, "helloo", 7+1);
    ASSERT_EQ(mvwprintw_fake.call_count, 1);
    //todo: fix the line below
    //ASSERT_STREQ(mvwprintw_fake.arg3_history[0], "helloo");
}

TEST_F(ClientLibTest, evaluate_complete_message)
{
    screen->input->i = 1;
    screen->input->buffer[0] = 'A';
    int e = evaluate_complete_message(screen, fake_fd);
    ASSERT_EQ(e, 0);
}

TEST_F(ClientLibTest, evaluate_complete_message_bad_fd)
{
    screen->input->i = 1;
    screen->input->buffer[0] = 'A';
    int e = evaluate_complete_message(screen, BAD_FD);
    ASSERT_EQ(e, -1);
}


TEST_F(ClientLibTest, evaluate_complete_message_blank_message)
{
    int e = evaluate_complete_message(screen, fake_fd);
    ASSERT_EQ(e, 0);
}

TEST_F(ClientLibTest, evaluate_complete_message_command)
{
    screen->input->i = 2;
    screen->input->buffer[0] = '/';
    screen->input->buffer[1] = 'q';
    int e = evaluate_complete_message(screen, fake_fd);
    ASSERT_EQ(e, 0);
}

TEST_F(ClientLibTest, evaluate_complete_message_command_bad_fd)
{
    screen->input->i = 5;
    screen->input->buffer[0] = '/';
    screen->input->buffer[1] = 'n';
    screen->input->buffer[2] = 'i';
    screen->input->buffer[3] = 'c';
    screen->input->buffer[4] = 'k';
    screen->input->buffer[5] = ' ';
    int e = evaluate_complete_message(screen, BAD_FD);
    ASSERT_EQ(e, -1);
}

TEST_F(ClientLibTest, evaluate_complete_message_command_unknown)
{
    screen->input->i = 2;
    screen->input->buffer[0] = '/';
    screen->input->buffer[1] = ')';
    int e = evaluate_complete_message(screen, fake_fd);
    ASSERT_EQ(e, 0);
}

TEST_F(ClientLibTest, client_async_init)
{
    int e = client_async_init(get_socket());
    EXPECT_GE(e, 0);
}

TEST_F(ClientLibTest, server_message_handling)
{
    fill_fake_fd(fake_fd, "yoooo!", 6+1);

    int e = server_message_handling(msgs, fake_fd);
    EXPECT_EQ(e, 0);
    ASSERT_EQ(mvwprintw_fake.call_count, 1);
}

TEST_F(ClientLibTest, server_message_handling_trailing_nullbyte)
{
    fill_fake_fd(fake_fd, "yoooo!\0", 6+1+1);

    int e = server_message_handling(msgs, fake_fd);
    EXPECT_EQ(e, 0);
    ASSERT_EQ(mvwprintw_fake.call_count, 1);
}

TEST_F(ClientLibTest, server_message_handling_multiple)
{
    fill_fake_fd(fake_fd, "yoooo!\0msg2\0msg3!", 17+1);

    int e = server_message_handling(msgs, fake_fd);
    EXPECT_EQ(e, 0);
    ASSERT_EQ(mvwprintw_fake.call_count, 3);
}

TEST_F(ClientLibTest, server_message_handling_connection_closed)
{
    int e = server_message_handling(msgs, fake_fd);
    EXPECT_EQ(e, -1);
}

TEST_F(ClientLibTest, server_message_handling_bad_fd)
{
    int e = server_message_handling(msgs, BAD_FD);
    EXPECT_EQ(e, -1);
}

TEST_F(ClientLibTest, stdin_char_handling_bad_fd)
{
    fake_stdin("A");
    int a = stdin_char_handling(screen, fake_fd);
    EXPECT_EQ(a, 0);
    ASSERT_EQ(mvwprintw_fake.call_count, 1);
    restore_stdin();
}

TEST_F(ClientLibTest, stdin_char_handling_bad_fd_tab)
{
    fake_stdin("\t");
    int a = stdin_char_handling(screen, fake_fd);
    EXPECT_EQ(a, 0);
    ASSERT_EQ(mvwprintw_fake.call_count, 0);
    restore_stdin();
}

TEST_F(ClientLibTest, stdin_char_handling_bad_fd_del)
{
    fake_stdin("\x7f"); //DEL
    int a = stdin_char_handling(screen, fake_fd);
    EXPECT_EQ(a, 0);
    ASSERT_EQ(wdelch_fake.call_count, 1);
    restore_stdin();
}

TEST_F(ClientLibTest, stdin_char_handling_bad_fd_return)
{
    fake_stdin("\r");
    int a = stdin_char_handling(screen, fake_fd);
    EXPECT_EQ(a, 0);
    //todo: add assert to check if function is doing the good something
    restore_stdin();
}

TEST_F(ClientLibTest, stdin_char_handling_bad_max)
{
    fake_stdin("A");
    screen->input->i = MAXMSG_CLI - 1;
    int a = stdin_char_handling(screen, fake_fd);
    EXPECT_EQ(a, 0);
    ASSERT_EQ(mvwprintw_fake.call_count, 0);
    restore_stdin();
}
