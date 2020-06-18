set(W
    -g
    -pedantic
    -Wall
    -Wextra

    -Wunreachable-code
    -Wmissing-include-dirs
    -Wfloat-equal
    -Wwrite-strings
    -Wpointer-arith
    -Wcast-qual
    -Wcast-align
    -Wshadow
    #-Wredundant-decls
    -Wdouble-promotion
    -Winit-self
    -Wswitch-default
    -Wswitch-enum
    -Wundef
    -Winline
    -Wformat-security
    -Wformat=2
    -Wunused-macros
    -Wdate-time
    -Wpacked
    -Winvalid-pch
    -Wvla
    -Wdisabled-optimization
    -Wconversion
)

set(RUNTIME_CHECKS
    -fsanitize=address
    -fsanitize=float-cast-overflow
    -fsanitize=float-divide-by-zero
    -fsanitize=leak
    -fsanitize=undefined
    -fno-omit-frame-pointer
    -fno-optimize-sibling-calls
)

macro(package_add_test TESTNAME)
    # https://cliutils.gitlab.io/modern-cmake/chapters/testing/googletest.html
    add_executable(${TESTNAME} ${ARGN})

    target_link_libraries(${TESTNAME}
        ${RUNTIME_CHECKS}
        ${CURSES_LIBRARIES}
        cjson
        gtest
        gtest_main
        "--coverage"
    )

    target_compile_options(${TESTNAME}
        PUBLIC ${RUNTIME_CHECKS}
        PUBLIC ${W}
        PUBLIC "-fPIE"
        PUBLIC "--coverage"
    )
    # gtest_discover_tests replaces gtest_add_tests,
    # see https://cmake.org/cmake/help/v3.10/module/GoogleTest.html for more options to pass to it
    gtest_discover_tests(${TESTNAME}
        # set a working directory so your project root so that you can find test data via paths relative to the project root
        WORKING_DIRECTORY ${PROJECT_DIR}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
    )
    set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
endmacro()

add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(utils)
