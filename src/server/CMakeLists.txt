add_executable(server
    clients.c
    server.c
    server_lib.c
    server_network.c
    ../utils/asynchronous.c
    ../utils/log.c
    ../utils/utils.c
)

target_compile_options(server 
    PUBLIC ${W}
    PUBLIC ${RUNTIME_CHECKS}
)
target_link_libraries(server
    ${RUNTIME_CHECKS}
)
