set(SERVER_SOURCES
    clients.c
    network.c
    server.c
    server_lib.c
    ../utils/asynchronous.c
    ../utils/log.c
    ../utils/utils.c
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
