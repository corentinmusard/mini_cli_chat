set(CLIENT_SOURCES
    client.c
    client_lib.c
    client_network.c
    cli.c
    screen.c
    ../utils/asynchronous.c
    ../utils/log.c
    ../utils/utils.c
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
