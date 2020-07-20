# Binaries will be into bin/ directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Utils add the static library "common"
add_subdirectory(utils)

add_subdirectory(server)
add_subdirectory(client)
