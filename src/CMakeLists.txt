#binary will be into bin/ directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

#begin warning setup
if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(W
        -pedantic
        -Wall
        -Wextra
        -Wstrict-prototypes
        -Wmissing-prototypes
        -Wmissing-declarations
        -Wunreachable-code
        -Wold-style-definition
        -Wmissing-include-dirs
        -Wdeclaration-after-statement
        -Wjump-misses-init
        -Wlogical-op
        -Wnested-externs
        -Wfloat-equal
        -Wwrite-strings
        -Wpointer-arith
        -Wcast-qual
        -Wcast-align=strict
        -Wshadow
        -Wredundant-decls
        -Wdouble-promotion
        -Winit-self
        -Wswitch-default
        -Wswitch-enum
        -Wundef
        -Wlogical-op
        -Winline
        -Wformat-security
        -Wformat=2
        -Wunused-macros
        -Wbad-function-cast
        -Wdate-time
        -Wpacked
        -Winvalid-pch
        -Wvla
        -Wdisabled-optimization
        -Wconversion
    )

    set(RUNTIME_CHECKS
        -fsanitize=address
        -fsanitize=pointer-compare
        -fsanitize=leak
        -fsanitize=pointer-subtract
        -fsanitize=undefined
        -fsanitize=float-divide-by-zero
        -fsanitize=float-cast-overflow
        -fsanitize-address-use-after-scope
        -fno-omit-frame-pointer
        -fno-optimize-sibling-calls
    )
elseif(CMAKE_C_COMPILER_ID MATCHES "Clang")
    set(W
        -pedantic
        -Wall
        -Wextra
        -Weverything
        -Wno-padded
        -Wno-disabled-macro-expansion
    )
    set(RUNTIME_CHECKS
        -fsanitize=address
        -fsanitize=leak
        -fsanitize=undefined
        -fsanitize=float-divide-by-zero
        -fsanitize=float-cast-overflow
        -fsanitize-address-use-after-scope
        -fno-omit-frame-pointer
        -fno-optimize-sibling-calls
    )
endif()
#end warning setup

add_subdirectory(server)
add_subdirectory(client)
