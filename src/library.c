// #include <zlog.h>

#include "library.h"

bool lupra_init() {
    return true;
}

/*
bool lupra_init(const char* zlog_config_path) {
    const char* category = "lupra";

    if (dzlog_init(zlog_config_path, category) != 0) {
        fprintf(
            stderr,
            "dzlog_init() failed: try again after setting ZLOG_PROFILE_ERROR "
            "to a log file path to get detailed log output\n"
        );
        return false;
    }

    return true;
}
*/

bool lupra_destroy() {
    // zlog_fini();

    return true;
}
