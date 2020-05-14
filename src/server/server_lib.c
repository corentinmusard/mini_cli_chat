#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include "asynchronous.h"
#include "clients.h"
#include "log.h"
#include "server_lib.h"
#include "utils.h"

/**
 * Message sent by server to a client after sucessful connection
 */
#define CHAT_BANNER "Welcome to the basic chat!"

/**
 * Send message `buffer` of size `size` to each client in `clients`
 */
static void broadcast_message(const Clients *clients, const char *buffer, size_t size) {
    assert(clients && "should not be NULL");
    assert(buffer && "should not be NULL");
    assert(size > 0 && "should not be 0");
    assert(size <= MAXMSG_SERV && "message is too long");

    const Client *c = clients->head;
    while (c != NULL) {
        write(c->fd, buffer, size);
        c = c->next;
    }
}

int server_async_init(int server_fd) {
    assert(server_fd >= 0 && "should be a valid file descriptor");

    int epollfd = async_init();
    async_add(epollfd, server_fd, EPOLLIN);
    return epollfd;
}

/**
 * Remove client from clients' list and close client's fd
 */
static void disconnect_client(Client *c) {
    assert(c && "should not be NULL");
    assert(c->list && "should not be NULL");
    assert(c->fd && "should not be NULL");

    info("%s: connection closed\n", c->username);
    close(c->fd);
    delete_client(c);
}

/**
 * Send `fmt` to a client, log it
 * Behave like a call to `info` followed by a call to `write`
 */
__attribute__((__format__ (__printf__, 2, 3)))
static void send_fd(int fd, const char *fmt, ...) {
    assert(fd >= 0 && "should be a valid file descriptor");
    assert(fmt && "should not be NULL");

    char msg[MAXMSG_SERV] = {0};
    va_list args;
    va_start(args, fmt);
    int len = vsnprintf(msg, MAXMSG_SERV, fmt, args);
    va_end(args);

    if (len >= MAXMSG_SERV) {
        info("%s: truncated output: len=%d, MAXMSG_SERV=%d\n", __func__, len, MAXMSG_SERV);
    }

    info(msg);
    write(fd, msg, sizeof(msg));
}

/**
 * Send `fmt` to each clients, log it
 * Behave like a call to `info` followed by a call to `broadcast_message`
 */
__attribute__((__format__ (__printf__, 2, 3)))
static void send_everyone(const Clients *clients, const char *fmt, ...) {
    assert(clients && "should not be NULL");
    assert(fmt && "should not be NULL");

    char msg[MAXMSG_SERV] = {0};
    va_list args;
    va_start(args, fmt);
    int len = vsnprintf(msg, MAXMSG_SERV, fmt, args);
    va_end(args);

    if (len >= MAXMSG_SERV) {
        info("%s: truncated output: len=%d, MAXMSG_SERV=%d\n", __func__, len, MAXMSG_SERV);
    }

    info(msg);
    broadcast_message(clients, msg, sizeof(msg));
}

/**
 * Execute special command starting with '/'
 */
static void special_command_handling(Client *c, char *buffer) {
    assert(c && "should not be NULL");
    assert(buffer && "should not be NULL");

    info("%s: %s\n", c->username, buffer);

    char *saveptr = NULL;
    const char *token = strtok_r(buffer, " ", &saveptr);
    if (token == NULL) {
        return;
    }

    if (strcmp("/nick", token) == 0) {
        const char *username = strtok_r(NULL, " ", &saveptr);
        if (username == NULL) {
            send_fd(c->fd, "/nick <nickname>\n");
            return;
        }
        //check availability
        if (!is_available_username(c->list, username)) {
            send_fd(c->fd, "Nickname not available\n");
            return;
        }

        // notify each clients
        send_everyone(c->list, "%s is now known as %s\n", c->username, username);

        // update username
        memset(c->username, 0, MAX_USERNAME_LENGTH);
        strncpy(c->username, username, MAX_USERNAME_LENGTH);
        c->username[MAX_USERNAME_LENGTH - 1] = '\0';
    } else { //unknown command
        send_fd(c->fd, "Unknown command\n");
    }
}

void client_message_handling(Client *c) {
    assert(c && "should not be NULL");

    char buffer[MAXMSG_CLI] = {0};
    ssize_t status = read(c->fd, buffer, sizeof(buffer));
    if (status == -1) { //error
        return;
    }

    if (status == 0) { //connection closed
        send_everyone(c->list, "%s: leave the server\n", c->username);
        disconnect_client(c);
        return;
    }

    if (buffer[0] == '/') { // special command
        special_command_handling(c, buffer);
        return;
    }
    // it's a normal message
    send_everyone(c->list, "%s: %s\n", c->username, buffer);
}

/**
 * Add client to clients' list and store client's fd
 * Return NULL on failure
 * Return client on success
 */
static Client* connect_client(Clients *clients, int fd, int epollfd) {
    assert(clients && "should not be NULL");
    assert(fd >= 0 && "should be a valid file descriptor");
    assert(epollfd >= 0 && "should be a valid file descriptor");

    async_add(epollfd, fd, EPOLLIN | EPOLLET);

    Client *c = add_client(clients, fd);
    info("%s: connection opened\n", c->username);
    return c;
}

void accept_client(Clients *clients, int epollfd, int server_fd) {
    assert(clients && "should not be NULL");
    assert(epollfd >= 0 && "should be a valid file descriptor");
    assert(server_fd >= 0 && "should be a valid file descriptor");

    int fd = accept4(server_fd, NULL, NULL, SOCK_NONBLOCK);
    if (fd == -1) {
        perror("accept4");
        return;
    }

    Client *c = connect_client(clients, fd, epollfd);
    if (c == NULL) {
        return;
    }

    ssize_t err = write(c->fd, CHAT_BANNER, sizeof(CHAT_BANNER));
    if (err == -1) {
        info("%s: error sending CHAT_BANNER\n", c->username);
        return;
    }
    send_everyone(c->list, "%s: join the server\n", c->username);
}
