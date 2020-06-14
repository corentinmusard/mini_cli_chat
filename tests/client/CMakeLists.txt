package_add_test(cli
    cli_test.cpp
    ../fake_functions.cpp
)

package_add_test(client_lib
    client_lib_test.cpp
    ../fake_functions.cpp
    ../utils.cpp
    ../../src/client/cli.c
    ../../src/client/screen.c
    ../../src/utils/asynchronous.c
    ../../src/utils/log.c
    ../../src/utils/utils.c
)

package_add_test(client_network
    network_test.cpp
    ../../src/client/client_network.c
    ../../src/server/server_network.c
)

package_add_test(screen
    screen_test.cpp
    ../fake_functions.cpp
    ../../src/client/cli.c
    ../../src/client/screen.c
)
