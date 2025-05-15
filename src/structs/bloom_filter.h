#pragma once

#include <stdint.h>

#include "bit_array.h"

/**
 * A bloom filter is a probabilistic set that can make the following guarantees:
 *   - It may or may not contain the item it says it does (false positives are possible)
 *   - It will definitely not contain the item it says it doesn't (false negatives are impossible)
 *
 * Bloom filters are useful when you need to do some preliminary set containment checks to filter out data you know
 * isn't in the filter. This allows you to do more costly queries on a smaller subset of the data later on.
 *
 * Values stored in the bloom filter are very memory efficient because they use a fixed-size bit array to store hashes
 * of values, rather than the values themselves.
 *
 * Items can't be deleted from a bloom filter.
 *
 * **Example**
 * ```c
 * bloom_filter bf;
 * bloom_filter_init(&bf, 100); // Max 100 values before false positive rate increases
 *
 * assert(!bloom_filter_check(&bf, "foo"));
 *
 * bloom_filter_add(&bf, "foo");
 * assert(bloom_filter_check(&bf, "foo"));
 *
 * bloom_filter_destroy(&bf);
 * ```
 */
typedef struct bloom_filter {
    /**
     * Number of hash functions to use
     * This should ideally be a minimum of 2
     * Hash functions use murmur3 but are simulated using the Kirsch & Mitzenmacher technique
     */
    size_t hash_count;

    /**
     * Bit array used to store bloom filter hashes
     */
    bit_array *bit_array;
} bloom_filter;

/**
 * Initialize the bloom filter
 *
 * @relates bloom_filter
 * @param[out] bf Bloom filter
 * @param[in] size Size of the bloom filter (should be a multiple of 32, or it will be rounded up to that)
 *   This should approximately match how many entries you want to store in the bloom filter. The larger
 *   the value, the fewer false positives there will be at the expense of increased memory usage.
 * @return true on success, false on failure
 */
bool bloom_filter_init(bloom_filter* bf, size_t size);

/**
 * Add a key to the bloom filter
 *
 * Time complexity: O(1)
 *
 * @relates bloom_filter
 * @param[in,out] bf Bloom filter
 * @param[in] key Key to add
 * @param[in] key_len Length of the key
 * @return true on success, false on failure
 */
bool bloom_filter_add(const bloom_filter* bf, const uint8_t* key, size_t key_len);

/**
 * Check if a key is in the bloom filter
 *
 * False positives are possible (meaning this can indicate that a key is a member when it's not)
 * False negatives are impossible (meaning this can't indicate that a key is not a member when it is)
 *
 * Time complexity: O(1)
 *
 * @relates bloom_filter
 * @param[in] bf Bloom filter
 * @param[in] key Key to check
 * @param[in] key_len Length of the key
 * @return true if the key is member (false positive possible), false if not (false negative impossible)
 */
bool bloom_filter_check(const bloom_filter* bf, const uint8_t* key, size_t key_len);

/**
 * Destroy the bloom filter
 *
 * Time complexity: O(1)
 *
 * @relates bloom_filter
 * @param[in,out] bf Bloom filter
 * @return true on success, false on failure
 */
bool bloom_filter_destroy(bloom_filter* bf);
