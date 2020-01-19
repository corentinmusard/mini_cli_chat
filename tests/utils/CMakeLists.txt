set(SOURCES
    ../../src/utils/asynchronous.c
    ../../src/utils/log.c
)

set(TESTS
    asynchronous_test.cpp
    log_test.cpp
)

add_executable(utils_test
    ${SOURCES}
    ${TESTS}
)

target_compile_options(utils_test
    PUBLIC "-fPIE"
    PUBLIC "--coverage"
    PUBLIC ${W}
)

target_link_libraries(utils_test
    GTest::GTest 
    GTest::Main
    "--coverage"
)

add_test(test_all
    utils_test
)