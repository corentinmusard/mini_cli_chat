#include <gtest/gtest.h>

#include "client/client_network.h"
#include "server/server_network.h"
#include "utils/utils.h"

class NetworkTest : public ::testing::Test
{
protected:
    void SetUp() override {
        fds = start_server(PORT);
    }

    void TearDown() override {
        stop_server(fds);
    }
    int fds;
};

TEST_F(NetworkTest, connect_client)
{
    int fd = connect_client("127.0.0.1", PORT);
    ASSERT_GT(fd, 0);
    disconnect_client(fd);
}

TEST_F(NetworkTest, connect_client_wrong_ip)
{
    int fd = connect_client("ABCD", PORT);
    EXPECT_EQ(fd, BAD_IP);
}

TEST_F(NetworkTest, connect_client_wrong_port)
{
    int fd = connect_client("127.0.0.1", 12345);
    EXPECT_EQ(fd, CONNECT_FAILED);
}
