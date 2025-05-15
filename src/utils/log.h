#pragma once

#include <string.h>
#include <errno.h>
// #include <zlog.h>

#ifndef NDEBUG
    #define log_debug(fmt, ...) \
        printf("DEBUG: " fmt "\n" __VA_OPT__(,) __VA_ARGS__)
#else
    #define log_debug(fmt, ...)
#endif

#define log_error(fmt, ...) \
    fprintf(stderr, "ERROR: " fmt "\n" __VA_OPT__(,) __VA_ARGS__)

#define log_perror(fmt, ...) \
    fprintf(stderr, "ERROR: " fmt "\n" __VA_OPT__(,) __VA_ARGS__); \
    fprintf(stderr, "%s\n", strerror(errno))
