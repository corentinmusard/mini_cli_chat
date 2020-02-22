set(TEST_NAME client_test)

set(SOURCES
    ../../src/client/cli.c
    ../../src/client/network.c
    ../../src/client/screen.c
    ../../src/server/network.c
    ../../src/utils/asynchronous.c
    ../../src/utils/log.c
)

set(TESTS
    network_test.cpp
    screen_test.cpp
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
    ${CURSES_LIBRARIES}
    #"ncurses_g" # for trace functions
)

add_test(${TEST_NAME} ${TEST_NAME})
