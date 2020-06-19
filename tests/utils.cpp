#include <assert.h>
#include <gtest/gtest.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "server/clients.h"
#include "utils/utils.h"
#include "utils.hpp"

int get_fake_fd(void) {
    FILE *f = tmpfile();
    if (f == NULL) {
        perror("tmpfile");
        exit(EXIT_FAILURE);
    }
    return fileno(f);
}

int get_socket(void) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}

char* fill_tmp_file(const char *buffer) {
    assert(buffer && "should not be NULL");

    char *s = tmpnam(NULL);
    FILE *f = fopen(s, "w");
    if (f == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fprintf(f, "%s", buffer);
    fclose(f);
    return s;
}

void read_equal_name_offset(const char *filename, const char *buffer, size_t offset) {
    assert(filename && "should not be NULL");
    assert(buffer && "should not be NULL");

    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    char out[MAXMSG_SERV] = {0};
    fread(out, 1, MAXMSG_SERV, f);
    EXPECT_STREQ(buffer+offset, out+offset);
}

void read_equal_name(const char *filename, const char *buffer) {
    read_equal_name_offset(filename, buffer, 0);
}

/**
 * Assert data read from `fd` equals to `buffer` or not
 */
static void read_expect(int fd, const char *buffer, bool equal) {
    assert(fd >= 0 && "should be a valid file descriptor");
    assert(buffer && "should not be NULL");

    off_t off = lseek(fd, 0, SEEK_SET);
    assert(off != -1);

    char out[MAXMSG_SERV] = {0};
    read(fd, out, MAXMSG_SERV);

    if (equal) {
        EXPECT_STREQ(buffer, out);
    } else {
        EXPECT_STRNE(buffer, out);
    }
}

void read_equal(int fd, const char *buffer) {
    read_expect(fd, buffer, true);
}

void read_not_equal(int fd, const char *buffer) {
    read_expect(fd, buffer, false);
}

void fill_fake_fd(int fd, const char * buffer, size_t size) {
    assert(fd >= 0 && "should be a valid file descriptor");
    assert(buffer && "should not be NULL");

    ssize_t n = write(fd, buffer, size);
    assert(n == (ssize_t)size);
    off_t off = lseek(fd, 0, SEEK_SET);
    assert(off != -1);
}

void fake_stdin(const char *buffer) {
    assert(buffer && "should not be NULL");

    char *s = tmpnam(NULL);
    FILE *f = fopen(s, "w");
    if (f == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fprintf(f, "%s", buffer);
    fclose(f);

    FILE *e = freopen(s, "r", stdin);
    assert(e);
    assert(fileno(stdin) == STDIN_FILENO);
}

void restore_stdin(void) {
    FILE *e = freopen("/dev/stdin", "r", stdin);
    assert(e);
    assert(fileno(stdin) == STDIN_FILENO);
}

// redefine NULL to remove a c++ error
#undef NULL
#define NULL ((void*)0)

void assert_is_empty(const Clients *clients) {
    assert(clients && "should not be NULL");

    EXPECT_EQ(clients->head, NULL);
    EXPECT_EQ(clients->nb, 0);
}

void assert_fds_are(const Clients *clients, const int *fds, size_t size) {
    assert(clients && "should not be NULL");
    assert(fds && "should not be NULL");

    Client *c = clients->head;
    ASSERT_EQ(clients->nb, size);
    for (size_t i = 0; i < size; i++) {
        EXPECT_EQ(c->fd, fds[i]);
        c = c->next;
    }
    EXPECT_EQ(c, NULL);
}
