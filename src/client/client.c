#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>

#include "asynchronous.h"
#include "client_lib.h"
#include "client_network.h"
#include "screen.h"
#include "utils.h"

int main(void) {
    if (register_sigint() == -1) {
        perror("register_sigint");
        return EXIT_FAILURE;
    }

    int sockfd = connect_client("127.0.0.1", PORT);
    if (sockfd < 0) {
        perror("connect_client");
        return EXIT_FAILURE;
    }

    int epollfd = client_async_init(sockfd);
    Screen *screen = screen_init();

    int err = 0;
    while (exit_not_wanted(err)) {
        refresh_screen(screen);

        struct epoll_event events[MAX_EVENTS];
        int nfds = wait_events(epollfd, events);

        for (int i = 0; i < nfds && err != -1; i++) {
            if (is_stdin(events[i].data.fd)) {
                err = stdin_char_handling(screen, sockfd);
            } else {
                err = server_message_handling(screen->msgs, sockfd);
            }
        }
    }

    free_screen(screen);
    disconnect_client(sockfd);
    return 0;
}
