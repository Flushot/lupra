#pragma once

#include <stdint.h>

#include "linked_list.h"
#include "../utils/value.h"

/**
 * Hash table entry
 */
typedef struct hash_table_entry {
    void* key;
    void* value;

    /**
     * If set to 1, then ht_destroy_entry() will automatically
     * be called on this entry when ht_destroy() was called
     */
    uint8_t must_destroy;
} hash_table_entry;

/**
 * Key hashing function
 *
 * @param[in] key Key to hash
 * @param[in] ht_size Size of the key
 * @return Hash value
 */
typedef uint32_t (*hash_table_key_hash_func)(const void* key, size_t ht_size);

/**
 * A closed addressed hash table (also commonly known as hash map, dictionary, or associative array) is a structure for
 * storing values at unique keys. These entries aren't ordered and there can only be one value for a given key.
 *
 * Hash tables are useful for lookup tables when you need to quickly find or store something that can be uniquely
 * identified by its key.
 *
 * **Example**
 * ```c
 * hash_table ht;
 * hash_table_init(&ht, 10, NULL, NULL); // Max 10 items before collisions increase
 *
 * hash_table_set(&ht, "foo", "one");
 * hash_table_set(&ht, "foo", "two");
 * hash_table_set(&ht, "bar", "three");
 *
 * char *foo = hash_table_get(&ht, "foo");
 * char *bar = hash_table_get(&ht, "bar");
 *
 * assert(strcmp(foo, "two") == 0);
 * assert(strcmp(bar, 'three") == 0);
 *
 * hash_table_destroy(&ht);
 * ```
 */
typedef struct hash_table {
    /**
     * Size of the index
     * This is NOT the size of all stored entries
     */
    size_t index_size;

    /**
     * Size of the total stored hash table entries
     */
    size_t entry_size;

    /**
     * Hash table index
     * Pointers to linked lists of entries
     */
    linked_list** index;

    /**
     * Key comparator function
     * Default: String comparator
     */
    value_cmp_func key_cmp;

    /**
     * Key hash function
     * Default: String hash function
     */
    hash_table_key_hash_func key_hash;
} hash_table;

/**
 * Initialize the hash table
 *
 * Time complexity: O(1)
 *
 * @relates hash_table
 * @param[out] ht Hash table
 * @param[in] size Index size
 * @param[in] key_cmp Key comparator (or NULL to use default)
 * @param[in] key_hash Key hash function (or NULL to use default)
 * @return true on success, false on failure
 */
bool hash_table_init(
    hash_table* ht,
    uint32_t size,
    value_cmp_func key_cmp,
    hash_table_key_hash_func key_hash
);

/**
 * Resize and rebuild the hash table
 *
 * Time complexity: O(n)
 *
 * @relates hash_table
 * @param[in,out] ht Hash table
 * @param[in] new_size New size of the hash table index
 * @return true on success, false on failure
 */
bool hash_table_rehash(hash_table* ht, uint32_t new_size);

/**
 * Initialize a new hash table entry by creating a copy of key/value
 *
 * Time complexity: O(1)
 *
 * @relates hash_table
 * @param[in] key Pointer to key (to copy)
 * @param[in] key_size Size of the key
 * @param[in] value Pointer to value (to copy)
 * @param[in] value_size Size of value
 * @return Hash table entry (must ht_destroy_entry() on this when finished with it)
 */
hash_table_entry* hash_table_init_entry(
    const void* key,
    size_t key_size,
    const void* value,
    size_t value_size
);

/**
 * Destroy a hash table entry created with ht_init_entry()
 *
 * Time complexity: O(1)
 *
 * @relates hash_table
 * @param[in,out] entry Hash table entry
 * @return true on success, false on failure
 */
bool hash_table_destroy_entry(hash_table_entry* entry);

/**
 * Set a value in the hash table
 *
 * Time complexity: O(1)
 *
 * @relates hash_table
 * @param[in,out] ht Hash table
 * @param[in] key Pointer to key
 * @param[in] value Pointer to value
 * @return true on success, false on failure
 */
bool hash_table_set(hash_table* ht, void* key, void* value);

/**
 * Set entry in the hash table
 *
 * Time complexity: O(1)
 *
 * @relates hash_table
 * @param[in,out] ht Hash table
 * @param[in] entry Entry to set
 * @return true on success, false on failure
 */
bool hash_table_set_entry(hash_table* ht, hash_table_entry* entry);

/**
 * Get entry from the hash table
 *
 * Time complexity: O(1)
 *
 * @relates hash_table
 * @param ht Hash table
 * @param key Key to get entry for
 * @return Entry or NULL if not found
 */
hash_table_entry* hash_table_get_entry(const hash_table* ht, const void* key);

/**
 * Get value from the hash table
 *
 * Time complexity: O(1)
 *
 * @relates hash_table
 * @param[in] ht Hash table
 * @param[in] key Entry key to get value for
 * @return Value pointer
 */
void* hash_table_get(const hash_table* ht, const void* key);

/**
 * Delete entry from the hash table
 *
 * Time complexity: O(1)
 *
 * @relates hash_table
 * @param[in,out] ht Hash table
 * @param[in] key Entry key to delete
 * @return true on success, false on failure
 */
bool hash_table_del(hash_table* ht, const void* key);

/**
 * Hash table iterator callback function
 *
 * @relates hash_table
 * @param[in] entry Iterated hash table entry
 * @param[in] index Iteration index
 * @param user_arg Optional user arg
 */
typedef void (*hash_table_iter_func)(
    hash_table_entry* entry,
    size_t index,
    void* user_arg
);

/**
 * Iterate hash table keys and values
 *
 * Time complexity: O(n)
 *
 * @relates hash_table
 * @param ht Hash table
 * @param[in] iter_func Iterator callback function
 * @param iter_func_user_arg Optional argument to pass to callback function
 * @return true on success, false on failure
 */
bool hash_table_iter(
    hash_table* ht,
    hash_table_iter_func iter_func,
    void* iter_func_user_arg
);

/**
 * Get all keys in the hash table
 *
 * Time complexity: O(n)
 *
 * @relates hash_table
 * @param[in] ht Hash table
 * @param[out] keys Pointer to array to store key pointers in
 * @return Number of keys, or -1 on failure
 */
size_t hash_table_keys(hash_table* ht, void** keys);

/**
 * Get all values in the hash table
 *
 * Time complexity: O(n)
 *
 * @relates hash_table
 * @param[in] ht Hash table
 * @param[out] values Pointer to array to store value pointers in
 * @return Number of values, or -1 on failure
 */
size_t hash_table_values(hash_table* ht, void** values);

/**
 * Get the number of entries in the hash table
 *
 * Time complexity: O(1)
 *
 * @relates hash_table
 * @param[in] ht Hash table
 * @return Number of entries, or -1 on failure
 */
size_t hash_table_size(const hash_table* ht);

/**
 * Destroy the hash table
 *
 * Time complexity: O(n)
 *
 * @relates hash_table
 * @param[in,out] ht hash table
 * @return true on success, false on failure
 */
bool hash_table_destroy(hash_table* ht);
