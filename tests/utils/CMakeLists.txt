set(TEST_NAME utils_test)

set(SOURCES
    ../utils.cpp
    ../../src/utils/asynchronous.c
    ../../src/utils/log.c
    ../../src/utils/utils.c
)

set(TESTS
    asynchronous_test.cpp
    log_test.cpp
    utils_test.cpp
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
