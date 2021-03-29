# Chat
[![MIT license](https://img.shields.io/badge/License-MIT-blue.svg)](https://lbesson.mit-license.org/)
[![HitCount](http://hits.dwyl.io/corentinmusard/mini_cli_chat.svg)](http://hits.dwyl.io/corentinmusard/mini_cli_chat)

A minimal clients/server chat application.
It uses Berkeley sockets, epoll, ncurses, cmake, google test.

## Build status
[![Build Status](https://www.travis-ci.org/corentinmusard/mini_cli_chat.svg?branch=master)](https://www.travis-ci.org/corentinmusard/mini_cli_chat)
[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=corentinmusard_mini_cli_chat&metric=alert_status)](https://sonarcloud.io/dashboard?id=corentinmusard_mini_cli_chat)
[![codecov](https://codecov.io/gh/corentinmusard/mini_cli_chat/branch/master/graph/badge.svg)](https://codecov.io/gh/corentinmusard/mini_cli_chat)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/0a07e19caa974f559eb7f2e901c311bb)](https://www.codacy.com/manual/corentinmusard/mini_cli_chat)

## Coding Process

Project is conforming to C11 and newer. It requires POSIX and Linux (for epoll). It uses modern CMake (or at least try to).

### Using [Design by contract](//barrgroup.com/embedded-systems/how-to/design-by-contract-for-embedded-software) (DbC)
Preconditions are checked with `assert`, postconditions are checked within tests.

### Tools used to ensure good code
-   [clang-tidy](//clang.llvm.org/extra/clang-tidy/)
-   [iwyu](//github.com/include-what-you-use/include-what-you-use)
-   [cppcheck](//github.com/danmar/cppcheck)
-   [fsanitize](//clang.llvm.org/docs/AddressSanitizer.html) runtime checks instead of `valgrind`

## Getting Started

### Build options
| Flag name         | Default | Explanation                                           |
| ----------------- | ------- | ----------------------------------------------------- |
| CLANG_TIDY_CHECKS | ON      | Run clang-tidy to lint the code                       |
| FSANITIZE_FLAGS   | ON      | Build with -fsanitize= flags                          |
| IWYU_CHECKS       | OFF     | Run include-what-you-use to find unnecessary #include |
| LWYU_CHECKS       | OFF     | Show unused linked libraries                          |

### Build

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

### Core features
-   [x] add nicknames support (default name, renaming and uniques nicknames)
-   [ ] add privates messages support
-   [ ] add channels support
-   [ ] add moderation support
-   [ ] add roles (modo/admin/muted) support

### Client
-   [x] display with ncurses
-   [ ] when screen is full, scroll messsages and display new messages
-   [ ] when input line is full, shift messsages and shift back when deleting or moving with the cursor keys
-   [ ] add command starting with a slash like /list, /me, /whoami, /whois
-   [ ] ncurses: add colors to nicknames, channels and more
-   [ ] add utf-8 support
-   [ ] add acknowledgment of receipt

### Server
-   [x] allow clients to connect
-   [x] forward messages received from a client to every clients connected to the server
-   [x] notify clients when someone join/leave the server
-   [ ] add a way to login into a specific nickname with a password
-   [ ] protect a nickname when it has been registred

### Asynchrone
-   [x] client: asynchrone gestion of output(new messages from server) and input(new messages from keyboard)
-   [x] server: asynchrone connections 

### Settings
-   [x] client: add setting configuration (nickname, server ip/port, log settings)
-   [x] server: add setting configuration (server port, log settings)

### Log system
-   [x] server: pseudo log system (print to stderr)
-   [x] add date to message
-   [x] create or append to a file client.log/server.log
-   [x] write every messages and errors to that file

### Build system
-   [x] use modern cmake
-   [x] add build options

### Code quality
-   [x] write function docstring
-   [x] automatize uses of linters (at compile time, at runtime and during Continuous Integration)
-   [ ] write documentation about client's commands starting with a slash
-   [ ] make unit tests
-   [ ] make integration tests
-   [ ] make performance tests
-   [ ] use a fuzzer
-   [ ] use portable code

### Optional features
-   [ ] make client/server works outside localhost
-   [ ] add IPv6 support
-   [ ] connection to server with ssl
-   [ ] make code modular with plugins
