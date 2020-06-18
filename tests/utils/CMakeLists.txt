package_add_test(asynchronous
    asynchronous_test.cpp
    ../utils.cpp
    ../../src/utils/asynchronous.c
)

package_add_test(log
    log_test.cpp
    ../utils.cpp
    ../../src/utils/log.c
)

package_add_test(utils
    utils_test.cpp
    ../../src/utils/utils.c
)

package_add_test(settings
    settings_test.cpp
    ../utils.cpp
    ../../src/utils/log.c
    ../../src/utils/settings.c

LIBS
    cjson
)
