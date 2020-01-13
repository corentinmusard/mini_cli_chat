set(SOURCES
    ../src/utils/asynchronous.c
    ../src/utils/log.c
)

set(TESTS
    log_test.cpp
    asynchronous_test.cpp
)

add_executable(unit_test
    ${TESTS}
    ${SOURCES}
)

target_compile_options(unit_test 
    PUBLIC "-fPIE"
    PUBLIC "--coverage"
)

target_link_libraries(unit_test
    GTest::GTest 
    GTest::Main
    "--coverage"
)

add_test(test_all
    unit_test
)