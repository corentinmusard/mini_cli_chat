#include <gtest/gtest.h>

#include "server/server_network.h"
#include "utils.h"

class NetworkTest : public ::testing::Test
{
    //nothing
};

TEST_F(NetworkTest, start_server)
{
    int fd = start_server(PORT+1);
    ASSERT_GT(fd, 0);
    stop_server(fd);
}

TEST_F(NetworkTest, start_server_bind_failed)
{
    int fd = start_server(PORT+1);
    int fd2 = start_server(PORT+1);
    ASSERT_EQ(fd2, BIND_FAILED);
    stop_server(fd);
}