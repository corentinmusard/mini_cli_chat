cmake_minimum_required(VERSION 3.1...3.18 FATAL_ERROR)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(mini_cli_chat
    VERSION 0.1
    DESCRIPTION "description of mini_cli_chat"
    LANGUAGES C CXX
)

# To build cJSON
add_subdirectory(3rd_party/cJSON)

find_package(Curses REQUIRED)

set(CMAKE_C_STANDARD 11) # or newer
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDANT_CODE ON)

if(CMAKE_VERSION VERSION_GREATER 3.3)
    option(IWYU_CHECKS "Run include-what-you-use to find unnecessary #include" OFF)
    find_program(IWYU_EXE
        NAMES "include-what-you-use"
        DOC "Path to include-what-you-use executable"
    )

    if(IWYU_EXE AND IWYU_CHECKS)
            set(CMAKE_C_INCLUDE_WHAT_YOU_USE
                "${IWYU_EXE}"
            )
    endif()
endif()

if(CMAKE_VERSION VERSION_GREATER 3.6)
    option(CLANG_TIDY_CHECKS "Run clang-tidy to lint the code" ON)
    find_program(CLANG_TIDY_EXE
        NAMES "clang-tidy"
        DOC "Path to clang-tidy executable"
    )

    if(CLANG_TIDY_EXE AND CLANG_TIDY_CHECKS)
            set(CMAKE_C_CLANG_TIDY
                "${CLANG_TIDY_EXE}"
                "-checks=*,-llvm-header-guard,-clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling,-android-cloexec-*,-hicpp-signed-bitwise,-cppcoreguidelines-*"
            )
    endif()
endif()

if(CMAKE_VERSION VERSION_GREATER 3.7)
    option(LWYU_CHECKS "Show unused linked libraries" OFF)

    if(LWYU_CHECKS)
        set(CMAKE_LINK_WHAT_YOU_USE ON)    
    endif()
endif()


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
            #-fanalyzer
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

option(FSANITIZE_FLAGS "Build with -fsanitize= flags" ON)
if(NOT FSANITIZE_FLAGS)
    set(RUNTIME_CHECKS ) # clear the flags    
endif()

# Build client and server
add_subdirectory(src)

# Unit tests
add_subdirectory(3rd_party/google-test)
find_package(GTest REQUIRED)

# Enable CTest testing
enable_testing()

# Add testing executables
add_subdirectory(tests)
