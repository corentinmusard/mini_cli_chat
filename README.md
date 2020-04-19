# Chat
[![MIT license](https://img.shields.io/badge/License-MIT-blue.svg)](https://lbesson.mit-license.org/)
[![HitCount](http://hits.dwyl.io/corentinmusard/mini_cli_chat.svg)](http://hits.dwyl.io/corentinmusard/mini_cli_chat)

A minimal clients/server chat application.
It uses Berkeley sockets, epoll, ncurses, cmake, google test.

## Build status
[![Build Status](https://www.travis-ci.org/corentinmusard/mini_cli_chat.svg?branch=master)](https://www.travis-ci.org/corentinmusard/mini_cli_chat)
[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=corentinmusard_mini_cli_chat&metric=alert_status)](https://sonarcloud.io/dashboard?id=corentinmusard_mini_cli_chat)
[![Coverage](https://sonarcloud.io/api/project_badges/measure?project=corentinmusard_mini_cli_chat&metric=coverage)](https://sonarcloud.io/dashboard?id=corentinmusard_mini_cli_chat)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/0a07e19caa974f559eb7f2e901c311bb)](https://www.codacy.com/manual/corentinmusard/mini_cli_chat)

## Coding Process

### Using [Design by contract](//barrgroup.com/embedded-systems/how-to/design-by-contract-for-embedded-software) (DbC)
Preconditions are checked with `assert`s, postconditions are checked within tests.

### Tools used to ensure good code
-   [scan-build](//clang-analyzer.llvm.org/scan-build.html)
-   [clang-tidy](//clang.llvm.org/extra/clang-tidy/)
-   [iwyu](//github.com/include-what-you-use/include-what-you-use)
-   [cppcheck](//github.com/danmar/cppcheck)
-   [fsanitize](//clang.llvm.org/docs/AddressSanitizer.html) runtime checks instead of `valgrind`

## Getting Started
To build both client and server binaries:
```sh
mkdir build
cd build
cmake ..
make
GTEST_COLOR=1 ctest # Optional, it runs tests
```
Binaries are built into `bin/` directory.

## Usage
None for now because it's not really usable.

## TODO LIST
### Features (ordered)
-   [x] add date to message
-   [x] server: forward messages received from a client to each clients connected to the server
-   [x] server: notify clients when someone join/leave server
-   [ ] add nicknames support
-   [ ] client: when screen is full, scroll messsages and display new messages
-   [ ] add privates messages support
-   [ ] add a way to login into a specific nickname with a password
-   [ ] server: protect a nickname when it has been registred
-   [ ] add channels support
-   [ ] add moderation support
-   [ ] add roles (modo/admin/muted) support

### Settings (in no particular order)
-   [ ] client: ncurses: add colors to nicknames, channels and more
-   [ ] client: add command via adding a slash like /list, /me, /whoami, /whois
-   [ ] client: add setting configuration (nickname, server ip/port, log settings)
-   [ ] server: add setting configuration (server port, log settings)

### Technical things (in no particular order)
-   [x] server: allow clients to connect
-   [x] server: epoll: asynchrone connections 
-   [x] client: epoll: asynchrone gestion of output(new messages from server) and input(new messages from keyboard)
-   [x] client: display with ncurses
-   [x] write function docstring
-   [ ] make tests
-   [ ] make code modular with plugins
-   [ ] use portable code (currently needing POSIX and GNU extentions)
-   [ ] add IPv6 support
-   [ ] connection to server with ssl
