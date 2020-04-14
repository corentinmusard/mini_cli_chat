#include <gtest/gtest.h>

#include "server/clients.h"

// redefine NULL to remove an error
#undef NULL
#define NULL ((void*)0)

#define FAILURE (-1)
#define SUCCESS 0

class ClientsTest : public ::testing::Test
{
protected:
    void SetUp() override {
        c = init_clients();
    }

    void TearDown() override {
        free_clients(c);
    }

    void assert_is_empty() {
        ASSERT_EQ(c->head, NULL);
        ASSERT_EQ(c->nb, 0);
    }

    void assert_fds_are(const int *fds, int size) {
        Client *cl = c->head;
        ASSERT_EQ(c->nb, size);
        for (int i = 0; i < size; i++) {
            ASSERT_EQ(cl->fd, fds[i]);
            cl = cl->next;
        }
        ASSERT_EQ(cl, NULL);
    }

    Clients *c;
};

TEST_F(ClientsTest, init_clients)
{
    assert_is_empty();
}

TEST_F(ClientsTest, add_client)
{
    int fds[1] = {2};

    ASSERT_EQ(add_client(c, 2), SUCCESS);

    assert_fds_are(fds, 1);
}

TEST_F(ClientsTest, add_client_bad_fd)
{
    ASSERT_EQ(add_client(c, -1), FAILURE);

    assert_is_empty();
}

TEST_F(ClientsTest, add_clients)
{
    int fds[4] = {3, 2, 4, 2};

    ASSERT_EQ(add_client(c, 2), SUCCESS);
    ASSERT_EQ(add_client(c, 4), SUCCESS);
    ASSERT_EQ(add_client(c, 2), SUCCESS);
    ASSERT_EQ(add_client(c, 3), SUCCESS);

    assert_fds_are(fds, 4);
}

TEST_F(ClientsTest, delete_client)
{
    add_client(c, 2);
    delete_client(c, 2);

    assert_is_empty();
}

TEST_F(ClientsTest, delete_client_empty)
{
    delete_client(c, 2);

    assert_is_empty();
}

TEST_F(ClientsTest, delete_client_wrong_fd)
{
    int fds[1] = {2};

    add_client(c, 2);
    delete_client(c, 4);

    assert_fds_are(fds, 1);
}

TEST_F(ClientsTest, delete_client_duplicate)
{
    int fds[2] = {1, 2};

    add_client(c, 2);
    add_client(c, 2);
    add_client(c, 1);
    delete_client(c, 2);

    assert_fds_are(fds, 2);
}

TEST_F(ClientsTest, delete_client_multiple_clients)
{
    int fds[3] = {1, 3, 4};

    add_client(c, 4);
    add_client(c, 2);
    add_client(c, 3);
    add_client(c, 1);
    delete_client(c, 2);

    assert_fds_are(fds, 3);
}

