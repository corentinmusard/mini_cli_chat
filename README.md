# Chat

A minimal clients/server chat application.
It uses Berkeley sockets, epoll, ncurses.

## TODO LIST:
First things to be done first

- [x] server: allow clients to connect
- [x] server: epoll: asynchrone connections 
- [x] client: epoll: asynchrone gestion of output(new messages from server) and input(new messages from keyboard)
- [ ] client: display with ncurses
- [ ] server: forward messages received by a client to every clients connected to the server
- [ ] add nicknames support
- [ ] connection to server with with ssl
- [ ] add channels support
- [ ] add privates messages support
- [ ] add a way to login into a specific nickname with a password
- [ ] server: protect a nickname when it has been registred
- [ ] add /me message support
- [ ] client: ncurses: add colors to nicknames, channels and more
- [ ] add logs options for clients and server
- [ ] make tests
- [ ] improve README.md
- [ ] add IPv6 support
