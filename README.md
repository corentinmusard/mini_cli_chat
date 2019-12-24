# Chat

A minimal clients/server chat application.
It uses Berkeley sockets, epoll, ncurses, cmake.

## Coding Process
### Tools used to ensure good code
- [scan-build](//clang-analyzer.llvm.org/scan-build.html)
- [clang-tidy](//clang.llvm.org/extra/clang-tidy/)
- [iwyu](//github.com/include-what-you-use/include-what-you-use)
- [cppcheck](//github.com/danmar/cppcheck)
- [fsanitize](//clang.llvm.org/docs/AddressSanitizer.html) runtime checks instead of `valgrind`

## Getting Started
To build both client and server binary:
```sh
mkdir build
cd build
cmake ..
make
make test # Optionnal, it runs tests
```
Binaries are built into `bin/` directory.

## Usage
None for now because it's not really usable.

## TODO LIST:
In no particular order

- [x] server: allow clients to connect
- [x] server: epoll: asynchrone connections 
- [x] client: epoll: asynchrone gestion of output(new messages from server) and input(new messages from keyboard)
- [x] client: display with ncurses
- [x] add date to message
- [x] server: forward messages received by a client to every clients connected to the server
- [x] write function docstring
- [ ] make tests
- [ ] add nicknames support
- [ ] server: notify clients when someone leave server
- [ ] add channels support
- [ ] add privates messages support
- [ ] add a way to login into a specific nickname with a password
- [ ] server: protect a nickname when it has been registred
- [ ] add roles (modo/admin/muted) support
- [ ] add moderation support
- [ ] client: ncurses: add colors to nicknames, channels and more
- [ ] add logs options for clients and server
- [ ] improve README.md
- [ ] connection to server with ssl
- [ ] add IPv6 support
- [ ] client: add setting configuration (nickname, server ip/port, log settings)
- [ ] server: add setting configuration (server port, log settings)
- [ ] client: add command via adding a slash like /quit, /me, /whoami, /whois
