# Add the executable "client"

add_executable(client
    client.c
    client_lib.c
    client_network.c
    client_settings.c
    cli.c
    screen.c
)

target_include_directories(client
PRIVATE
    ${CMAKE_SOURCE_DIR}/3rd_party
    ${CMAKE_SOURCE_DIR}/src
)

target_compile_options(client 
PRIVATE
    ${W}
    ${RUNTIME_CHECKS}
)

target_link_libraries(client
PRIVATE
    cjson
    common
    ${CURSES_LIBRARIES}
    ${RUNTIME_CHECKS}
)
