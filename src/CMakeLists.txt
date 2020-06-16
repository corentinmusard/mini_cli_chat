#binaries will be into bin/ directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

#Warning setup
if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    if (CMAKE_C_COMPILER_VERSION VERSION_LESS 4.9.0)
        message(FATAL_ERROR "Require gcc 4.9.0 or newer.")
    endif()

    set(W
        -g
        -pedantic
        -Wall
        -Wextra

        -Wbad-function-cast
        -Wcast-align
        -Wcast-qual
        -Wconversion
        -Wdate-time
        -Wdisabled-optimization
        -Wdouble-promotion
        -Wfloat-equal
        -Wformat-security
        -Wformat=2
        -Winit-self
        -Winline
        -Winvalid-pch
        -Wjump-misses-init
        -Wstrict-prototypes
        -Wlogical-op
        -Wmissing-declarations
        -Wmissing-include-dirs
        -Wmissing-prototypes
        -Wnested-externs
        -Wold-style-definition
        -Wpacked
        -Wpointer-arith
        -Wredundant-decls
        -Wshadow
        -Wswitch-default
        -Wswitch-enum
        -Wundef
        -Wunreachable-code
        -Wunused-macros
        -Wvla
        -Wwrite-strings
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

    if (CMAKE_C_COMPILER_VERSION VERSION_GREATER_EQUAL 6)
        set(W
            ${W}
            -Wduplicated-cond
        )
    endif()

    if (CMAKE_C_COMPILER_VERSION VERSION_GREATER_EQUAL 7)
        set(W
            ${W}
            -Walloc-zero
            -Walloca
            -Wduplicated-branches
        )
    endif()

    if (CMAKE_C_COMPILER_VERSION VERSION_GREATER_EQUAL 8)
        set(W
            ${W}
            -Wcast-align=strict
            -Wmultistatement-macros
        )
        set(RUNTIME_CHECKS
            ${RUNTIME_CHECKS}
            -fsanitize=pointer-compare
            -fsanitize=pointer-subtract
        )
    endif()

    # No new warning for gcc 9

    if (CMAKE_C_COMPILER_VERSION VERSION_GREATER_EQUAL 10)
        set(W
            ${W}
            -Warith-conversion
        )
    endif()

elseif(CMAKE_C_COMPILER_ID MATCHES "Clang")
    set(W
        -pedantic
        -Wall
        -Wextra
        -Weverything

        -Wno-disabled-macro-expansion
        -Wno-padded
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
else()
    message(FATAL_ERROR "Require gcc or clang.")
endif()

add_subdirectory(server)
add_subdirectory(client)
