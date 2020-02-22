#include <stdio.h>
#include <sys/epoll.h>

#include "asynchronous.h"
#include "client_lib.h"
#include "network.h"
#include "screen.h"
#include "utils.h"

int main(void) {
        Screen *screen = screen_init();
        int epollfd;
        int sockfd;
        int err = 0;

        sockfd = connect_client("127.0.0.1", PORT);
        if (sockfd < 0) {
                perror("connect_client");
                goto clean;
        }

        if (register_sigint() == -1) {
                perror("register_sigint");
                goto clean;
        }

        epollfd = client_async_init(sockfd);
        if (epollfd == -1) {
                perror("client_async_init");
                goto clean;
        }

        while (exit_not_wanted(err)) {
                struct epoll_event events[MAX_EVENTS];
                int nfds;

                refresh_screen(screen);

                nfds = wait_events(epollfd, events);
                if (nfds == -1) {
                        perror("wait_event");
                        err = -1;
                }

                for (int i = 0; i < nfds && err != -1; i++) {
                        if (is_stdin(events[i].data.fd)) {
                                err = stdin_char_handling(screen, sockfd);
                        } else {
                                err = server_message_handling(screen->msgs, sockfd);
                        }
                }
        }

clean:
        disconnect_client(sockfd);
        free_screen(screen);
        return 0;
}
