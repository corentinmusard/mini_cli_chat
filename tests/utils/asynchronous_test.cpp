#include <gtest/gtest.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <limits.h>

#include "asynchronous.h"
#include "tests/utils.hpp"

class AsyncTest : public ::testing::Test
{
protected:
    void SetUp() override {
        epollfd = async_init();
    }

    void TearDown() override {
        close(epollfd);
    }

    void add_socket() {
        int sockfd = get_socket();
        async_add(epollfd, sockfd, 0);
    }

    int epollfd;
};

TEST_F(AsyncTest, async_init)
{
    SUCCEED(); // already tested in SetUp
}

TEST_F(AsyncTest, async_add_add_socket)
{
    add_socket();
}

TEST_F(AsyncTest, DISABLED_async_add_epollin)
{
}

TEST_F(AsyncTest, DISABLED_async_add_epollin_epollet)
{
}

TEST_F(AsyncTest, wait_events)
{
    add_socket();

    struct epoll_event events[MAX_EVENTS];
    int e = wait_events(epollfd, events);
    ASSERT_EQ(e, 1);
}

TEST_F(AsyncTest, wait_events_multiple)
{
    add_socket();
    add_socket();
    add_socket();

    struct epoll_event events[MAX_EVENTS];
    int e = wait_events(epollfd, events);
    ASSERT_EQ(e, 3);
}
