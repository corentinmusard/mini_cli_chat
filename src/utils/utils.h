#ifndef UTILS_H
#define UTILS_H
#ifdef __cplusplus
extern "C" {
#endif

/*********************************** client and server ***********************************/

/**
 * server port
 */
#define PORT 5000

/**
 * Maximum length of a message
 */
#define MAXMSG 100

/**
 * Maximum events to be returned by epoll_wait
 * See man epoll_wait(2) for more infos
 */
#define MAX_EVENTS 10

/************************************** client only **************************************/

//None

/************************************** server only **************************************/

/**
 * Maximum length to which the queue of pending connections for server_sock_fd may grow. 
 * See man listen(2) for more infos
 */
#define LISTEN_BACKLOG 50

/**
 * Message sent by server to a client after sucessful connection
 */
#define CHAT_BANNER "Welcome to the basic chat!"

#ifdef __cplusplus
}
#endif
#endif
