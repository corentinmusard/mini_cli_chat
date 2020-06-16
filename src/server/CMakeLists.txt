add_executable(server
    clients.c
    server.c
    server_lib.c
    server_network.c
    server_settings.c
    ../utils/asynchronous.c
    ../utils/log.c
    ../utils/utils.c
)

target_compile_options(server 
    PUBLIC ${W}
    PUBLIC ${RUNTIME_CHECKS}
)
target_link_libraries(server
    cjson
    ${RUNTIME_CHECKS}
)
