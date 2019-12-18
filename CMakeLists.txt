cmake_minimum_required(VERSION 3.1...3.15)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(mini_cli_chat VERSION 0.1
                DESCRIPTION "description of mini_cli_chat"
                LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDANT_CODE ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_subdirectory(src)

# Unit tests
add_subdirectory(3rd_party/google-test)

# enable CTest testing
enable_testing()

# Add a testing executable
unset(CMAKE_RUNTIME_OUTPUT_DIRECTORY) # reset default output directory
add_subdirectory(tests)
