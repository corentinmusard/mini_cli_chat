#ifndef ASYNCHRONOUS_H
#define ASYNCHRONOUS_H

int async_init(void);
int register_event(int epollfd, int fd, unsigned int events);

#endif
