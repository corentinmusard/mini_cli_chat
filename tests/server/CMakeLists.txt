set(TEST_NAME server_test)

set(SOURCES
    ../../src/server/clients.c
    ../../src/server/network.c
)

set(TESTS
    clients_test.cpp
    network_test.cpp
)

add_executable(${TEST_NAME}
    ${SOURCES}
    ${TESTS}
)

target_compile_options(${TEST_NAME}
    PUBLIC "-fPIE"
    PUBLIC "--coverage"
    PUBLIC ${W}
)

target_link_libraries(${TEST_NAME}
    GTest::GTest 
    GTest::Main
    "--coverage"
)

add_test(${TEST_NAME} ${TEST_NAME})
