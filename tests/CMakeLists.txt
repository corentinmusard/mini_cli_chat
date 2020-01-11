set(SOURCES
    ../src/utils/log.c
)

add_executable(unit_test
    log_test.cpp
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