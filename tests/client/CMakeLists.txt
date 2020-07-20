package_add_test(cli
    cli_test.cpp
    ../fake_functions.cpp

LIBS
    ${CURSES_LIBRARIES}
)

package_add_test(client_lib
    client_lib_test.cpp
    ../fake_functions.cpp
    ../utils.cpp
    ../../src/client/cli.c
    ../../src/client/screen.c

LIBS
    common
    ${CURSES_LIBRARIES}
)

package_add_test(client_network
    network_test.cpp
    ../../src/client/client_network.c
    ../../src/server/server_network.c

DEFINE
    -D_POSIX_C_SOURCE=1 # POSIX.1 1990
)

package_add_test(screen
    screen_test.cpp
    ../fake_functions.cpp
    ../../src/client/cli.c
    ../../src/client/screen.c

LIBS
    ${CURSES_LIBRARIES}
)

package_add_test(client_settings
    client_settings_test.cpp
    ../utils.cpp
    ../../src/client/client_settings.c

LIBS
    cjson
    common
)
