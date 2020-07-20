function(package_add_test TESTNAME)
    # https://cliutils.gitlab.io/modern-cmake/chapters/testing/googletest.html
    set(options ) # empty
    set(oneValueArgs ) # empty
    set(multiValueArgs DEFINE LIBS)
    cmake_parse_arguments(MY "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    add_executable(${TESTNAME} ${MY_UNPARSED_ARGUMENTS})

    target_compile_definitions(${TESTNAME}
    PRIVATE
        ${MY_DEFINE}
    )

    target_include_directories(${TESTNAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/3rd_party
    )

    target_link_libraries(${TESTNAME}
    PRIVATE
        ${MY_LIBS}
        GTest::GTest
        GTest::Main
        "--coverage"
    )

    target_compile_options(${TESTNAME}
    PRIVATE
        -Wall
        -Wextra
        "-fPIE"
        "--coverage"
    )
    # gtest_discover_tests replaces gtest_add_tests,
    # See https://cmake.org/cmake/help/v3.10/module/GoogleTest.html for more options to pass to it
    gtest_discover_tests(${TESTNAME}
        # Set a working directory so your project root so that you can find test data via paths relative to the project root
        WORKING_DIRECTORY ${PROJECT_DIR}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
    )
    set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
endfunction()

add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(utils)
