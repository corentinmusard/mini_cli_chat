#include <assert.h>
#include <gtest/gtest.h>
#include <stdio.h>
#include <unistd.h>

#include "server/server_lib.c"
#include "server/server_network.h"
#include "tests/fake_functions.hpp"
#include "tests/utils.hpp"

FAKE_VOID_FUNC_VARARG(info, const char *, ...)

class ServerLibTest : public ::testing::Test
{
protected:
    void SetUp() override {
        epollfd = server_async_init(get_socket());
        clients = init_clients();

        // resets fake functions
        RESET_FAKE(info)
        // reset common FFF internal structures
        FFF_RESET_HISTORY()
    }

    void TearDown() override {
        free_clients(clients);
    }

    Client* add_fake_client(void) {
        static int i = 0;
        i++;

        Client *c = add_client(clients, i);
        c->fd = get_fake_fd();
        return c;
    }

    void assert_broadcast_message_equal(const char *buffer) {
        Client *c = clients->head;
        for (int i = 0; i < clients->nb; i++) {
            read_equal(c->fd, buffer);
            c = c->next;
        }
    }

    Clients *clients;
    int epollfd;
};

TEST_F(ServerLibTest, broadcast_message_no_client)
{
    char buffer[MAXMSG_SERV] = "Hello world!";

    broadcast_message(clients, buffer, MAXMSG_SERV);
    assert_broadcast_message_equal(buffer);
}

TEST_F(ServerLibTest, broadcast_message_one_client)
{
    add_fake_client();

    char buffer[MAXMSG_SERV] = "Hello world!";
    broadcast_message(clients, buffer, MAXMSG_SERV);
    assert_broadcast_message_equal(buffer);
}

TEST_F(ServerLibTest, broadcast_message_multiple_clients)
{
    add_fake_client();
    add_fake_client();
    add_fake_client();

    char buffer[MAXMSG_SERV] = "Hello world!";
    broadcast_message(clients, buffer, MAXMSG_SERV);
    assert_broadcast_message_equal(buffer);
}

TEST_F(ServerLibTest, server_async_init)
{
    EXPECT_GE(epollfd, 0);
}

TEST_F(ServerLibTest, disconnect_client)
{
    Client *c = add_client(clients, 2);
    disconnect_client(c);

    assert_is_empty(clients);
}

TEST_F(ServerLibTest, disconnect_client_info)
{
    Client *c = add_client(clients, 2);
    disconnect_client(c);

    EXPECT_EQ(info_fake.call_count, 1);
}

TEST_F(ServerLibTest, disconnect_client_duplicate)
{
    Client *c = add_client(clients, 2);
    add_client(clients, 2);
    add_client(clients, 1);
    disconnect_client(c);

    assert_fds_are(clients, (const int[]){1, 2}, 2);
}

TEST_F(ServerLibTest, disconnect_client_multiple_clients)
{
    add_client(clients, 4);
    Client *c = add_client(clients, 2);
    add_client(clients, 3);
    add_client(clients, 1);
    disconnect_client(c);

    assert_fds_are(clients, (const int[]){1, 3, 4}, 3);
}

TEST_F(ServerLibTest, send_fd)
{
    Client *c = add_fake_client();

    char buffer[MAXMSG_SERV] = "Hello world!";
    send_fd(c->fd, "%s", buffer);

    read_equal(c->fd, buffer);
}

TEST_F(ServerLibTest, send_fd_info)
{
    Client *c = add_fake_client();

    char buffer[MAXMSG_SERV] = "Hello world!";
    send_fd(c->fd, "%s", buffer);

    EXPECT_EQ(info_fake.call_count, 1);
}

TEST_F(ServerLibTest, send_fd_truncate)
{
    Client *c = add_fake_client();

    char buffer[MAXMSG_SERV+100] = {0};
    memset(buffer, 0x41, MAXMSG_SERV+99);
    send_fd(c->fd, "%s", buffer);

    read_not_equal(c->fd, buffer);
    EXPECT_EQ(info_fake.call_count, 2);
}

TEST_F(ServerLibTest, send_everyone_one_client)
{
    add_fake_client();

    char buffer[MAXMSG_SERV] = "Hello world!";
    send_everyone(clients, "%s", buffer);

    assert_broadcast_message_equal(buffer);
}

TEST_F(ServerLibTest, send_everyone_multiple_clients)
{
    add_fake_client();
    add_fake_client();
    add_fake_client();

    char buffer[MAXMSG_SERV] = "Hello world!";
    send_everyone(clients, "%s", buffer);

    assert_broadcast_message_equal(buffer);
    EXPECT_EQ(info_fake.call_count, 1);
}

TEST_F(ServerLibTest, send_everyone_info)
{
    add_fake_client();

    char buffer[MAXMSG_SERV] = "Hello world!";
    send_everyone(clients, "%s", buffer);

    EXPECT_EQ(info_fake.call_count, 1);
}

TEST_F(ServerLibTest, send_everyone_info_truncate)
{
    add_fake_client();

    char buffer[MAXMSG_SERV+100] = {0};
    memset(buffer, 0x41, MAXMSG_SERV+99);
    send_everyone(clients, "%s", buffer);

    EXPECT_EQ(info_fake.call_count, 2);
}

TEST_F(ServerLibTest, command_nick_null)
{
    char buffer[MAXMSG_CLI] = "/nick ";
    char *saveptr = NULL;
    const char *token = strtok_r(buffer, " ", &saveptr);
    assert(token != NULL);

    Client *c = add_fake_client();
    strncpy(c->username, "default_nick", MAX_USERNAME_LENGTH);

    command_nick(c, saveptr);

    EXPECT_STREQ(c->username, "default_nick");
    read_equal(c->fd, "/nick <nickname>\n");
}

TEST_F(ServerLibTest, command_nick_not_available)
{
    char buffer[MAXMSG_CLI] = "/nick used";
    char *saveptr = NULL;
    const char *token = strtok_r(buffer, " ", &saveptr);
    assert(token != NULL);

    Client *c = add_fake_client();
    strncpy(c->username, "used", MAX_USERNAME_LENGTH);

    command_nick(c, saveptr);

    EXPECT_STREQ(c->username, "used");
    read_equal(c->fd, "Nickname not available\n");
}

TEST_F(ServerLibTest, command_nick_available)
{
    char buffer[MAXMSG_CLI] = "/nick new_name";
    char *saveptr = NULL;
    const char *token = strtok_r(buffer, " ", &saveptr);
    assert(token != NULL);

    Client *c = add_fake_client();
    strncpy(c->username, "old_name", MAX_USERNAME_LENGTH);

    command_nick(c, saveptr);

    EXPECT_STREQ(c->username, "new_name");
    read_equal(c->fd, "old_name is now known as new_name\n");
}

TEST_F(ServerLibTest, command_nick_available_broadcast)
{
    char buffer[MAXMSG_CLI] = "/nick new_name";
    char *saveptr = NULL;
    const char *token = strtok_r(buffer, " ", &saveptr);
    assert(token != NULL);

    add_fake_client();
    add_fake_client();
    Client *c = add_fake_client();
    strncpy(c->username, "old_name", MAX_USERNAME_LENGTH);

    command_nick(c, saveptr);

    assert_broadcast_message_equal("old_name is now known as new_name\n");
}

TEST_F(ServerLibTest, special_command_handling_info)
{
    char buffer[MAXMSG_CLI] = "/nick";
    Client *c = add_fake_client();

    special_command_handling(c, buffer);

    ASSERT_GT(info_fake.call_count, 1);
    EXPECT_STREQ(info_fake.arg0_history[0], "%s: %s\n");
}

TEST_F(ServerLibTest, special_command_handling_unknown_command)
{
    char buffer[MAXMSG_CLI] = "/my,custom~command";
    Client *c = add_fake_client();

    special_command_handling(c, buffer);

    read_equal(c->fd, "Unknown command /my,custom~command\n");
    EXPECT_EQ(info_fake.call_count, 2);
}

TEST_F(ServerLibTest, special_command_handling_nick)
{
    char buffer[MAXMSG_CLI] = "/nick new_name";
    Client *c = add_fake_client();
    strncpy(c->username, "old_name", MAX_USERNAME_LENGTH);

    special_command_handling(c, buffer);

    EXPECT_STREQ(c->username, "new_name");
    read_equal(c->fd, "old_name is now known as new_name\n");
}

TEST_F(ServerLibTest, client_message_handling)
{
    const char buffer[MAXMSG_CLI] = "Hello world!";
    Client *c = add_fake_client();
    strncpy(c->username, "default_nick", MAX_USERNAME_LENGTH);
    Client *c2 = add_fake_client();

    fill_fake_fd(c->fd, buffer, sizeof(buffer));
    client_message_handling(c);

    EXPECT_GT(info_fake.call_count, 0);
    read_equal(c2->fd, "default_nick: Hello world!\n");
}

TEST_F(ServerLibTest, client_message_handling_bad_fd)
{
    Client *c = add_fake_client();
    c->fd = BAD_FD;
    Client *c2 = add_fake_client();

    client_message_handling(c);

    EXPECT_EQ(info_fake.call_count, 0);
    read_equal(c2->fd, "");
}

TEST_F(ServerLibTest, client_message_handling_leave)
{
    Client *c = add_fake_client();
    strncpy(c->username, "default_nick", MAX_USERNAME_LENGTH);
    Client *c2 = add_fake_client();

    client_message_handling(c);

    EXPECT_GT(info_fake.call_count, 0);
    read_equal(c2->fd, "default_nick: leave the server\n");
}

TEST_F(ServerLibTest, client_message_handling_command)
{
    const char buffer[MAXMSG_CLI] = "/nick new_name";
    Client *c = add_fake_client();
    strncpy(c->username, "default_nick", MAX_USERNAME_LENGTH);
    Client *c2 = add_fake_client();

    fill_fake_fd(c->fd, buffer, sizeof(buffer));
    client_message_handling(c);

    EXPECT_GT(info_fake.call_count, 0);
    read_equal(c2->fd, "default_nick is now known as new_name\n");
}

TEST_F(ServerLibTest, DISABLED_connect_client)
{
    int fd = get_fake_fd();
    Client *c = connect_client(clients, fd, epollfd);

    //get_client(clients, c->fd); //it succeeds or assert_fail()
    EXPECT_EQ(info_fake.call_count, 1);
    read_equal(c->fd, "%s: connection opened\n");
}
