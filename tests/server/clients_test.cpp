#include <gtest/gtest.h>

#include "server/clients.h"
#include "tests/utils.hpp"

class ClientsTest : public ::testing::Test
{
protected:
    void SetUp() override {
        clients = init_clients();
    }

    void TearDown() override {
        free_clients(clients);
    }

    Clients *clients;
};

TEST_F(ClientsTest, init_clients)
{
    assert_is_empty(clients);
}

TEST_F(ClientsTest, add_client)
{
    add_client(clients, 2);

    assert_fds_are(clients, (const int[]){2}, 1);
}

TEST_F(ClientsTest, add_client_multiple)
{
    add_client(clients, 2);
    add_client(clients, 4);
    add_client(clients, 2);
    add_client(clients, 3);

    assert_fds_are(clients, (const int[]){3, 2, 4, 2}, 4);
}

TEST_F(ClientsTest, add_client_return)
{
    EXPECT_EQ(add_client(clients, 2)->fd, 2);
}

TEST_F(ClientsTest, delete_client_fd)
{
    add_client(clients, 2);
    delete_client_fd(clients, 2);

    assert_is_empty(clients);
}

TEST_F(ClientsTest, delete_client_fd_duplicate)
{
    add_client(clients, 2);
    add_client(clients, 2);
    add_client(clients, 1);
    delete_client_fd(clients, 2);

    assert_fds_are(clients, (const int[]){1, 2}, 2);
}

TEST_F(ClientsTest, delete_client_fd_multiple_clients)
{
    add_client(clients, 4);
    add_client(clients, 2);
    add_client(clients, 3);
    add_client(clients, 1);
    delete_client_fd(clients, 2);

    assert_fds_are(clients, (const int[]){1, 3, 4}, 3);
}

TEST_F(ClientsTest, get_client)
{
    add_client(clients, 2);

    EXPECT_EQ(get_client(clients, 2)->fd, 2);
}

TEST_F(ClientsTest, get_client_middle)
{
    add_client(clients, 3);
    add_client(clients, 2);
    add_client(clients, 1);

    EXPECT_EQ(get_client(clients, 2)->fd, 2);
}

TEST_F(ClientsTest, get_client_last)
{
    add_client(clients, 3);
    add_client(clients, 2);
    add_client(clients, 1);

    EXPECT_EQ(get_client(clients, 3)->fd, 3);
}

TEST_F(ClientsTest, delete_client)
{
    add_client(clients, 2);
    delete_client(get_client(clients, 2));

    assert_is_empty(clients);
}

TEST_F(ClientsTest, delete_client_duplicate)
{
    add_client(clients, 2);
    add_client(clients, 2);
    add_client(clients, 1);

    delete_client(get_client(clients, 2));

    assert_fds_are(clients, (const int[]){1, 2}, 2);
}

TEST_F(ClientsTest, delete_client_multiple_clients)
{
    add_client(clients, 4);
    add_client(clients, 2);
    add_client(clients, 3);
    add_client(clients, 1);

    delete_client(get_client(clients, 2));

    assert_fds_are(clients, (const int[]){1, 3, 4}, 3);
}

TEST_F(ClientsTest, is_available_username_false_first)
{
    Client *c = add_client(clients, 2);
    EXPECT_FALSE(is_available_username(clients, c->username));
}

TEST_F(ClientsTest, is_available_username_false_multiple)
{
    add_client(clients, 3);
    Client *c = add_client(clients, 2);
    add_client(clients, 1);

    EXPECT_FALSE(is_available_username(clients, c->username));
}

TEST_F(ClientsTest, is_available_username_true_empty)
{
    EXPECT_TRUE(is_available_username(clients, "__random_name__"));
}

TEST_F(ClientsTest, is_available_username_true_multiple)
{
    add_client(clients, 3);
    add_client(clients, 2);
    add_client(clients, 1);

    EXPECT_TRUE(is_available_username(clients, "__random_name__"));
}
