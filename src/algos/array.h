#pragma once

#include <math.h>
#include <stdlib.h>

/**
 * Binary search index of needle (value) in haystack (array)
 *
 * This only works with numeric values that can be compared by inequality
 * operators over an already sorted array.
 *
 * Time complexity: O(lg n)
 *
 * This is a macro instead of a function so that it supports generic types
 * in a way that doesn't require an array of void* pointers.
 *
 * **Example**
 * ```c
 * size_t arr_size = 6;
 * int arr[] = {1, 2, 3, 4, 5, 6};
 *
 * size_t index = ARRAY_BINARY_SEARCH(int, arr, arr_size, 4); // Find item 4
 * assert(index == 3);
 * ```
 *
 * @param elem_type Array element type
 * @param haystack Sorted array
 * @param haystack_len Size of array (or upper bound index for search)
 * @param needle Value to search for
 * @return Index or -1 if not found
 */
#define array_binary_search(elem_type, haystack, haystack_len, needle) ({ \
    size_t lo = 0; \
    size_t hi = haystack_len; \
    size_t retval = -1; \
    do { \
        const size_t mid = (size_t)floor((double)lo + (double)(hi - lo) / 2); \
        const elem_type elem = haystack[mid]; \
        if (elem == needle) { \
            retval = mid; \
            break; \
        } \
        else if (elem < needle) { lo = mid + 1; } \
        else { hi = mid; } \
    } \
    while (hi > lo); \
    retval; \
})
