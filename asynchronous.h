#ifndef __ASYNCHRONOUS_H__
#define __ASYNCHRONOUS_H__

int async_init(void);
int register_event(int epollfd, int fd, unsigned int events);

#endif
