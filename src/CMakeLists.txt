set(W
    -pedantic
    -Wall
    -Wextra
    -Wstrict-prototypes
    -Wmissing-prototypes
    -Wmissing-declarations
    -Wunreachable-code
    -Wold-style-definition
    -Wmissing-include-dirs
    -Wdeclaration-after-statement
    -Wjump-misses-init
    -Wlogical-op
    -Wnested-externs
    -Wfloat-equal
    -Wwrite-strings
    -Wpointer-arith
    -Wcast-qual
    -Wcast-align=strict
    -Wshadow
    -Wredundant-decls
    -Wdouble-promotion
    -Winit-self
    -Wswitch-default
    -Wswitch-enum
    -Wundef
    -Wlogical-op
    -Winline
    -Wformat-security
    -Wformat=2
    -Wunused-macros
    -Wbad-function-cast
    -Wdate-time
    -Wpacked
    -Winvalid-pch
    -Wvla
    -Wdisabled-optimization
    -Wconversion
)

set(RUNTIME_CHECKS
    -fsanitize=address
    -fsanitize=pointer-compare
    -fsanitize=leak
    -fsanitize=pointer-subtract
    -fsanitize=undefined
    -fsanitize=float-divide-by-zero
    -fsanitize=float-cast-overflow
    -fsanitize-address-use-after-scope
    -fno-omit-frame-pointer
    -fno-optimize-sibling-calls
)

#begin server
set(SERVER_SOURCES
    server.c
    asynchronous.c
    log.c
)

add_executable(server ${SERVER_SOURCES})

target_compile_definitions(server 
    PUBLIC "_GNU_SOURCE"
)
target_compile_options(server 
    PUBLIC ${W}
    PUBLIC ${RUNTIME_CHECKS}
)
target_link_libraries(server
    ${RUNTIME_CHECKS}
)
#end server

#begin client
set(CLIENT_SOURCES
    client.c
    asynchronous.c
    cli.c
    client_lib.c
    log.c
)

add_executable(client ${CLIENT_SOURCES})

target_compile_options(client 
    PUBLIC ${W}
    PUBLIC ${RUNTIME_CHECKS}
)
target_link_libraries(client
    ${CURSES_LIBRARIES}
    ${RUNTIME_CHECKS}
)
#end client

