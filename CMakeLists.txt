cmake_minimum_required(VERSION 3.1...3.15)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(mini_cli_chat VERSION 0.1
                DESCRIPTION "description of mini_cli_chat"
                LANGUAGES C CXX)

set(CMAKE_C_STANDARD 99) # or newer
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
# add_definitions(-D_POSIX_C_SOURCE=1) # POSIX.1 1990 
add_definitions(-D_POSIX_C_SOURCE=200809L) # POSIX 2008
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDANT_CODE ON)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

# To make #include easier
include_directories(.)
include_directories(src)
include_directories(src/utils)

# Build client and server
add_subdirectory(src)

# Unit tests
add_subdirectory(3rd_party/google-test)

# To include fff/fff.h
include_directories(3rd_party)

# Enable CTest testing
enable_testing()

# Add testing executables
add_subdirectory(tests)
