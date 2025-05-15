#include <string.h>

#include "value.h"

int value_cmp_string(const void* a, const void* b) {
    if (a == NULL || b == NULL) {
        if (a == NULL && b != NULL) {
            // a < b
            return -1;
        }

        if (a != NULL && b == NULL) {
            // a > b
            return 1;
        }

        // a == b
        return 0;
    }

    return strcmp(a, b);
}

int value_cmp_int(const void* a, const void* b) {
    if (a == NULL || b == NULL) {
        if (a == NULL && b != NULL) {
            // a < b
            return -1;
        }

        if (a != NULL && b == NULL) {
            // a > b
            return 1;
        }

        // a == b
        return 0;
    }

    const int val_a = *(int *)a;
    const int val_b = *(int *)b;

    if (val_a == val_b) {
        return 0;
    }

    if (val_a > val_b) {
        return 1;
    }

    // val_a < val_b
    return -1;
}
