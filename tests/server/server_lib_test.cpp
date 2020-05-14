#include <assert.h>
#include <gtest/gtest.h>
#include <stdio.h>
#include <unistd.h>

#include "server/server_lib.c"
#include "tests/utils.hpp"

class ServerLibTest : public ::testing::Test
{
protected:
    void SetUp() override {
        clients = init_clients();
    }

    void TearDown() override {
        free_clients(clients);
    }

    void add_fake_client(void) {
        static int i = 0;
        i++;

        Client *c = add_client(clients, i);
        c->fd = get_fake_fd();
    }

    void assert_broadcast_message_equal(const char *buffer) {
        Client *c = clients->head;
        for (int i = 0; i < clients->nb; i++) {
            read_equal(c->fd, buffer);
            c = c->next;
        }
    }

    Clients *clients;
    void *shmem;
};

TEST_F(ServerLibTest, broadcast_message_no_client)
{
    char buffer[MAXMSG_SERV] = "Hello world!";

    broadcast_message(clients, buffer, MAXMSG_SERV);
    assert_broadcast_message_equal(buffer);
}

TEST_F(ServerLibTest, broadcast_message_one_client)
{
    char buffer[MAXMSG_SERV] = "Hello world!";

    add_fake_client();

    broadcast_message(clients, buffer, MAXMSG_SERV);
    assert_broadcast_message_equal(buffer);
}

TEST_F(ServerLibTest, broadcast_message_multiple_clients)
{
    char buffer[MAXMSG_SERV] = "Hello world!";

    add_fake_client();
    add_fake_client();
    add_fake_client();

    broadcast_message(clients, buffer, MAXMSG_SERV);
    assert_broadcast_message_equal(buffer);
}
