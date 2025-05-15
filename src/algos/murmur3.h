#pragma once

#include <stdlib.h>
#include <stdint.h>

/**
 * MurmurHash 3
 *
 * Time complexity: O(n)
 *
 * @param[in] key Key to hash
 * @param[in] len Length of the key
 * @param[in] seed Hash seed
 * @return Hash value
 */
uint32_t murmur3(const uint8_t* key, size_t len, uint32_t seed);
