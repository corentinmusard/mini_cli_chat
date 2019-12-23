set(SERVER_SOURCES
    server.c
    ../utils/asynchronous.c
    ../utils/log.c
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