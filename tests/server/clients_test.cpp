#include <gtest/gtest.h>

#include "server/clients.h"

// redefine NULL to remove an error
#undef NULL
#define NULL ((void*)0)

class ClientsTest : public ::testing::Test
{
protected:
    void SetUp() override {
        clients = init_clients();
    }

    void TearDown() override {
        free_clients(clients);
    }

    void assert_is_empty() {
        ASSERT_EQ(clients->head, NULL);
        ASSERT_EQ(clients->nb, 0);
    }

    void assert_fds_are(const int *fds, int size) {
        Client *c = clients->head;
        ASSERT_EQ(clients->nb, size);
        for (int i = 0; i < size; i++) {
            ASSERT_EQ(c->fd, fds[i]);
            c = c->next;
        }
        ASSERT_EQ(c, NULL);
    }

    Clients *clients;
};

TEST_F(ClientsTest, init_clients)
{
    assert_is_empty();
}

TEST_F(ClientsTest, add_client)
{
    int fds[1] = {2};

    add_client(clients, 2);

    assert_fds_are(fds, 1);
}

TEST_F(ClientsTest, add_client_multiple)
{
    int fds[4] = {3, 2, 4, 2};

    add_client(clients, 2);
    add_client(clients, 4);
    add_client(clients, 2);
    add_client(clients, 3);

    assert_fds_are(fds, 4);
}

TEST_F(ClientsTest, add_client_return)
{
    Client *c = add_client(clients, 2);
    ASSERT_EQ(c->fd, 2);
}

TEST_F(ClientsTest, delete_client_fd)
{
    add_client(clients, 2);
    delete_client_fd(clients, 2);

    assert_is_empty();
}

TEST_F(ClientsTest, delete_client_fd_duplicate)
{
    int fds[2] = {1, 2};

    add_client(clients, 2);
    add_client(clients, 2);
    add_client(clients, 1);
    delete_client_fd(clients, 2);

    assert_fds_are(fds, 2);
}

TEST_F(ClientsTest, delete_client_fd_multiple_clients)
{
    int fds[3] = {1, 3, 4};

    add_client(clients, 4);
    add_client(clients, 2);
    add_client(clients, 3);
    add_client(clients, 1);
    delete_client_fd(clients, 2);

    assert_fds_are(fds, 3);
}

TEST_F(ClientsTest, get_client)
{
    add_client(clients, 2);
    Client *client = get_client(clients, 2);

    ASSERT_EQ(client->fd, 2);
}

TEST_F(ClientsTest, get_client_middle)
{
    add_client(clients, 3);
    add_client(clients, 2);
    add_client(clients, 1);
    Client *client = get_client(clients, 2);

    ASSERT_EQ(client->fd, 2);
}

TEST_F(ClientsTest, get_client_last)
{
    add_client(clients, 3);
    add_client(clients, 2);
    add_client(clients, 1);
    Client *client = get_client(clients, 3);

    ASSERT_EQ(client->fd, 3);
}

TEST_F(ClientsTest, delete_client)
{
    add_client(clients, 2);
    Client *c = get_client(clients, 2);
    delete_client(c);

    assert_is_empty();
}

TEST_F(ClientsTest, delete_client_duplicate)
{
    int fds[2] = {1, 2};

    add_client(clients, 2);
    add_client(clients, 2);
    add_client(clients, 1);

    Client *c = get_client(clients, 2);
    delete_client(c);

    assert_fds_are(fds, 2);
}

TEST_F(ClientsTest, delete_client_multiple_clients)
{
    int fds[3] = {1, 3, 4};

    add_client(clients, 4);
    add_client(clients, 2);
    add_client(clients, 3);
    add_client(clients, 1);

    Client *c = get_client(clients, 2);
    delete_client(c);

    assert_fds_are(fds, 3);
}

TEST_F(ClientsTest, is_available_username_false_first)
{
    Client *c = add_client(clients, 2);
    ASSERT_FALSE(is_available_username(clients, c->username));
}

TEST_F(ClientsTest, is_available_username_false_multiple)
{
    add_client(clients, 3);
    Client *c = add_client(clients, 2);
    add_client(clients, 1);

    ASSERT_FALSE(is_available_username(clients, c->username));
}

TEST_F(ClientsTest, is_available_username_true_empty)
{
    ASSERT_TRUE(is_available_username(clients, "__random_name__"));
}

TEST_F(ClientsTest, is_available_username_true_multiple)
{
    add_client(clients, 3);
    add_client(clients, 2);
    add_client(clients, 1);

    ASSERT_TRUE(is_available_username(clients, "__random_name__"));
}
