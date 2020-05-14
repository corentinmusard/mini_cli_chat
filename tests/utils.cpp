#include <assert.h>
#include <gtest/gtest.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

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

void read_equal(int fd, const char *buffer) {
    char out[MAXMSG_SERV] = {0};
    off_t off = lseek(fd, 0, SEEK_SET);
    assert(off != -1);

    read(fd, out, MAXMSG_SERV);
    ASSERT_STREQ(buffer, out);
}

void fill_fake_fd(int fd, const char * buffer, size_t size) {
    ssize_t n = write(fd, buffer, size);
    assert(n == (ssize_t)size);
    off_t off = lseek(fd, 0, SEEK_SET);
    assert(off != -1);
}

void fake_stdin(const char *buffer) {
    char *s = tmpnam(NULL);
    FILE *f = fopen(s, "w");
    if (f == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fprintf(f, "%s", buffer);
    fclose(f);

    freopen(s, "r", stdin);
    assert(fileno(stdin) == STDIN_FILENO);
}

void restore_stdin(void) {
    freopen("/dev/stdin", "r", stdin);
    assert(fileno(stdin) == STDIN_FILENO);
}
