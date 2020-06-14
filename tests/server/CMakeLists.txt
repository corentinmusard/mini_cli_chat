package_add_test(clients
    clients_test.cpp
    ../../src/server/clients.c
    ../utils.cpp
)

package_add_test(server_network
    network_test.cpp
    ../../src/server/server_network.c
)

package_add_test(server_lib
    server_lib_test.cpp
    ../fake_functions.cpp
    ../utils.cpp
    ../../src/server/clients.c
    ../../src/utils/asynchronous.c
)
