#pragma once

/**
* Value comparator function
 *
 * @param[in] a First value to compare
 * @param[in] b Second value to compare
 * @return Comparison result (same behavior is strcmp):
 *   0  when a == b
 *   >0 when a > b
 *   <0 when a < b
 */
typedef int (*value_cmp_func)(const void* a, const void* b);

/**
 * Value comparator function for strings
 * {@see value_cmp_func}
 */
int value_cmp_string(const void* a, const void* b);

/**
 * Value comparator function for ints
 * {@see value_cmp_func}
 */
int value_cmp_int(const void* a, const void* b);
