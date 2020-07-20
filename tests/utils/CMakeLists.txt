package_add_test(asynchronous
    asynchronous_test.cpp
    ../utils.cpp
    ../../src/utils/asynchronous.c
)

package_add_test(log
    log_test.cpp
    ../utils.cpp
    ../../src/utils/log.c

DEFINE
    -D_POSIX_C_SOURCE=1 # POSIX.1 1990
)

package_add_test(utils
    utils_test.cpp
    ../../src/utils/utils.c

DEFINE
    -D_POSIX_C_SOURCE=1 # POSIX.1 1990
)

package_add_test(settings
    settings_test.cpp
    ../utils.cpp
    ../../src/utils/log.c
    ../../src/utils/settings.c

DEFINE
    -D_POSIX_C_SOURCE=1 # POSIX.1 1990

LIBS
    cjson
)
