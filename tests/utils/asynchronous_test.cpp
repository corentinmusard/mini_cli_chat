#include <gtest/gtest.h>
#include <sys/socket.h>
#include <unistd.h>
#include <limits.h>

#include "asynchronous.h"

class AsyncTest : public ::testing::Test
{
protected:
    void SetUp() override {
        epollfd = async_init();
    }

    void TearDown() override {
        close(epollfd);
    }

    int epollfd;
};

TEST_F(AsyncTest, async_init)
{
    ASSERT_GT(epollfd, 0);
}

TEST_F(AsyncTest, register_event_add_socket)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ASSERT_NE(sockfd, -1);

    int e = register_event(epollfd, sockfd, 0);
    ASSERT_EQ(e, 0);
}

TEST_F(AsyncTest, register_event_add_invalid_fd)
{
    int e = register_event(epollfd, INT_MAX, 0);
    ASSERT_EQ(e, -1);
}

TEST_F(AsyncTest, register_event_epollin)
{
    FAIL();
}

TEST_F(AsyncTest, register_event_epollin_epollet)
{
    FAIL();
}
