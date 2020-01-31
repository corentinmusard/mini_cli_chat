set(SOURCES
    ../../src/client/cli.c
    ../../src/client/client_lib.c
    ../../src/utils/asynchronous.c
    ../../src/utils/log.c
)

set(TESTS
    client_lib_test.cpp
)

add_executable(client_test
    ${SOURCES}
    ${TESTS}
)

target_compile_options(client_test
    PUBLIC "-fPIE"
    PUBLIC "--coverage"
    PUBLIC ${W}
)

target_link_libraries(client_test
    GTest::GTest 
    GTest::Main
    "--coverage"
    ${CURSES_LIBRARIES}
)

add_test(client_test client_test)
