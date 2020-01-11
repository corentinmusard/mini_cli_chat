set(CLIENT_SOURCES
    client.c
    client_lib.c
    cli.c
    ../utils/asynchronous.c
    ../utils/log.c
)

add_executable(client ${CLIENT_SOURCES})

target_compile_options(client 
    PUBLIC ${W}
    PUBLIC ${RUNTIME_CHECKS}
    PUBLIC  "--coverage"
)
target_link_libraries(client
    ${CURSES_LIBRARIES}
    ${RUNTIME_CHECKS}
     "--coverage"
)
