# Add the static library "common"

add_library(common STATIC
    asynchronous.c
    log.c
    settings.c
    utils.c
)

target_compile_definitions(common
PRIVATE
    -D_POSIX_C_SOURCE=1 # POSIX.1 1990
)

target_include_directories(common
PRIVATE
    ${CMAKE_SOURCE_DIR}/3rd_party
)

target_compile_options(common 
PRIVATE
    ${W}
)

target_link_libraries(common
PRIVATE
    cjson
)
