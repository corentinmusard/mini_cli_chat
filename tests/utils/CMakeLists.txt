package_add_test(asynchronous
    asynchronous_test.cpp
    ../utils.cpp
    ../../src/utils/asynchronous.c
)

package_add_test(log
    log_test.cpp
    ../../src/utils/log.c
)

package_add_test(utils
    utils_test.cpp
    ../../src/utils/utils.c
)
