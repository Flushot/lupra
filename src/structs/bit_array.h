#pragma once

#include <stdint.h>

/**
 * A bit array (commonly known as bit map, bit set, bit string, or bit vector) is an array that compactly stores bits.
 *
 * Bit arrays are useful when you have a lot of boolean values that you want to store using minimal memory.
 *
 * **Example**
 * ```c
 * bit_array ba;
 * bit_array_init(&ba, 10); // 10 possible values
 *
 * bit_array_set(&ba, 8); // Turn bit 8 on
 * assert(bit_array_test(&ba, 8) == true);
 *
 * bit_array_clear(&ba, 8); // Turn bit 8 off
 * assert(bit_array_test(&ba, 8) == false);
 *
 * bit_array_destroy(&ba);
 * ```
 */
typedef struct bit_array {
    /**
     * Max items that can be stored in the bit array (measured in bits)
     * This is always going to be a multiple of 32 (to match the uint32_t type in the data_bits array)
     */
    size_t size_bits;

    /**
     * Bit array
     * https://www.cs.emory.edu/%7Echeung/Courses/255/Syllabus/1-C-intro/bit-array.html
     */
    uint32_t* bit_array;
} bit_array;

/**
 * Initialize the bit array
 *
 * @relates bit_array
 * @param[out] ba Bit array
 * @param[in] size Max items to store in bit array
 * @return true on success, false on failure
 */
bool bit_array_init(bit_array *ba, size_t size);

/**
 * Set a bit to 1 in the bit array
 *
 * Time complexity: O(1)
 *
 * @relates bit_array
 * @param[in,out] ba Bit array
 * @param[in] k Bit to set (will wrap w/ modulo size_bits)
 */
void bit_array_set(bit_array *ba, uint32_t k);

/**
 * Set a bit to 0 in the bit array
 *
 * Time complexity: O(1)
 *
 * @relates bit_array
 * @param[in,out] ba Bit array
 * @param k Bit to clear (will wrap w/ modulo size_bits)
 */
void bit_array_clear(bit_array *ba, uint32_t k);

/**
 * Test if a bit is set to 1 in the bit array
 *
 * Time complexity: O(1)
 *
 * @relates bit_array
 * @param[in] ba Bit array
 * @param[in] k Bit to test (will wrap w/ modulo size_bits)
 * @return true if bit is set, false otherwise
 */
bool bit_array_test(const bit_array *ba, uint32_t k);

/**
 * Destroy the bit array
 *
 * Time complexity: O(1)
 *
 * @relates bit_array
 * @param[in,out] ba Bit array
 * @return true on success, false on failure
 */
bool bit_array_destroy(bit_array *ba);
