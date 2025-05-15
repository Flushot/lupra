#include <stdlib.h>
#include <stdio.h>

#include "bit_array.h"
#include "../utils/log.h"

bool bit_array_init(bit_array *ba, const size_t size) {
    const size_t array_elem_size = sizeof(uint32_t);
    const size_t array_elem_size_bits = array_elem_size * 8;

    // Round up to the nearest multiple of array_elem_size_bits
    ba->size_bits = ((size + (array_elem_size_bits - 1)) / array_elem_size_bits) * array_elem_size_bits;

    // Calculate number of array elements required
    size_t array_elem_count = ba->size_bits / array_elem_size_bits;
    if (ba->size_bits % array_elem_size_bits > 0) {
        array_elem_count += 1; // Equivalent of ceil int division
    }

    ba->bit_array = malloc(array_elem_count * array_elem_size);
    if (ba->bit_array == NULL) {
        log_perror("malloc() failed");
        return false;
    }

    memset(ba->bit_array, 0, array_elem_count * array_elem_size);

    return true;
}

void bit_array_set(bit_array *ba, const uint32_t k) {
    ba->bit_array[k / ba->size_bits] |= 1U << (k % ba->size_bits);
}

void bit_array_clear(bit_array *ba, const uint32_t k) {
    ba->bit_array[k / ba->size_bits] &= ~(1U << (k % ba->size_bits));
}

bool bit_array_test(const bit_array *ba, const uint32_t k) {
    return (ba->bit_array[k / ba->size_bits] & (1U << (k % ba->size_bits))) != 0;
}

bool bit_array_destroy(bit_array *ba) {
    if (ba->bit_array != NULL) {
        free(ba->bit_array);
        ba->bit_array = nullptr;
    }

    ba->size_bits = 0;

    return true;
}
