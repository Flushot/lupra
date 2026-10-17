#pragma once

#include <stddef.h>
#include "./hash_table.h"

/**
 * A hash set contains only unique values.
 *
 * The backing data structure is a hash table, which gives O(1) lookups.
 *
 * **Example**
 *     ```c
 *     hash_set hs;
 *     hash_set_init(&hs, 5, nullptr, nullptr); // Max 10 items before hash collisions increase
 *
 *     hash_set_add(&hs, "foo");
 *     hash_set_add(&hs, "bar");
 *
 *     assert(hash_set_contains(&hs, "foo"));
 *     assert(hash_set_contains(&hs, "bar"));
 *     assert(!hash_set_contains(&hs, "spangle"));
 *     ```
 */
typedef struct hash_set {
    /**
     * Backing hash table
     *
     * Keys are hash set values, and values are nullptr
     */
    hash_table table;
} hash_set;

/**
 * Initialize the hash set
 *
 * Takes same parameters as hash_table_init
 *
 * Time complexity: O(1)
 *
 * @relates hash_set
 * @param[in,out] hs Hash set to initialize
 * @param[in] size Index size
 * @param[in] key_cmp Key comparator (or NULL to use default)
 * @param[in] key_hash Key hash function (or NULL to use default)
 * @return true on success, false on failure
 */
bool hash_set_init(
    hash_set* hs,
    uint32_t size,
    value_cmp_func key_cmp,
    hash_table_key_hash_func key_hash
);

/**
 * Resize and rebuild the hash set
 *
 * Time complexity: O(n)
 *
 * @relates hash_set
 * @param[in,out] hs Hash set
 * @param[in] new_size New size of the hash set index
 * @return true on success, false on failure
 */
bool hash_set_rehash(hash_set* hs, uint32_t new_size);

/**
 * Destroy the hash set
 *
 * @relates hash_set
 * @param hs Hash set
 * @return true on success, false on failure
 */
bool hash_set_destroy(hash_set* hs);

/**
 * Add a value to the hash set
 *
 * @relates hash_set
 * @param[in,out] hs Hash set
 * @param[in] value Value to add
 * @return true on success, false on failure
 */
bool hash_set_add(hash_set* hs, void* value);

/**
 * Check if a value exists in the hash set
 *
 * @relates hash_set
 * @param[in] hs Hash set
 * @param[in] value Value to check
 * @return true if value exists, false otherwise
 */
bool hash_set_contains(hash_set* hs, void* value);

/**
 * Remove a value from the hash set
 *
 * @relates hash_set
 * @param[in,out] hs Hash set
 * @param[in] value Value to remove
 * @return true on success, false on failure
 */
bool hash_set_del(hash_set* hs, void* value);

/**
 * Get the number of values in the hash set
 *
 * @relates hash_set
 * @param[in] hs Hash set
 * @return Number of values in the hash set
 */
size_t hash_set_size(hash_set* hs);
