# Add the executable "server"

add_executable(server
    clients.c
    server.c
    server_lib.c
    server_network.c
    server_settings.c
)

target_compile_definitions(server
PRIVATE
    -D_POSIX_C_SOURCE=1 # POSIX.1 1990
)

target_include_directories(server
PRIVATE
    ${CMAKE_SOURCE_DIR}/3rd_party
    ${CMAKE_SOURCE_DIR}/src
)

target_compile_options(server 
PRIVATE
    ${W}
    ${RUNTIME_CHECKS}
)

target_link_libraries(server
PRIVATE
    cjson
    common
    ${RUNTIME_CHECKS}
)
