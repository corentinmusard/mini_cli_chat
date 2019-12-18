add_executable(unit_tests unit_tests.cpp)

target_link_libraries(unit_tests
    GTest::GTest 
    GTest::Main
)

add_test(test_all unit_tests)