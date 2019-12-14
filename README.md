# Chat

A minimal clients/server chat application.
It uses Berkeley sockets, epoll, ncurses.

## Coding Process
### Tools used to ensure good code
- [scan-build](//clang-analyzer.llvm.org/scan-build.html)
- [clang-tidy](//clang.llvm.org/extra/clang-tidy/)
- [iwyu](//github.com/include-what-you-use/include-what-you-use)
- [cppcheck](//github.com/danmar/cppcheck)
- `fsanitize` runtime checks instead of `valgrind`

## TODO LIST:
First things to be done first

- [x] server: allow clients to connect
- [x] server: epoll: asynchrone connections 
- [x] client: epoll: asynchrone gestion of output(new messages from server) and input(new messages from keyboard)
- [x] client: display with ncurses
- [x] add date to message
- [ ] make tests
- [ ] write function docstring
- [ ] server: forward messages received by a client to every clients connected to the server
- [ ] server: notify clients when someone leave server
- [ ] add nicknames support
- [ ] connection to server with ssl
- [ ] add channels support
- [ ] add privates messages support
- [ ] add a way to login into a specific nickname with a password
- [ ] server: protect a nickname when it has been registred
- [ ] add /me message support
- [ ] add roles (modo/admin/muted) support
- [ ] add moderation support
- [ ] client: ncurses: add colors to nicknames, channels and more
- [ ] add logs options for clients and server
- [ ] improve README.md
- [ ] add IPv6 support
