#include <stdio.h>

#include "hash_table.h"
#include "../algos/murmur3.h"
#include "../utils/log.h"

/**
 * Array builder iterator user_arg
 */
struct hash_table_array_builder_arg {
    size_t index;

    /**
     * Array to store items in
     */
    void** items;
};

/**
 * Get a hash table index pointer for the given key
 * Hashes the key then computes its index offset
 *
 * @param[in] ht Hash table
 * @param[in] key Key to compute index for
 * @return Computed index
 */
static inline size_t find_index(const hash_table* ht, const void* key) {
    return (*ht->key_hash)(key, ht->index_size) % (ht->index_size - 1);
}

/**
 * Default hashing function (murmur3)
 *
 * @param[in] key Key to hash
 * @param[in] ht_size Size of hash table index
 * @return Hash value
 */
static uint32_t default_key_hash(const void* key, size_t ht_size) {
    static uint32_t hash_seed = -1;
    if (hash_seed == -1) {
        hash_seed = rand();
    }

    return murmur3(key, strlen(key), hash_seed) % (ht_size - 1);
}

bool hash_table_init(
    hash_table* ht,
    const uint32_t size,
    const value_cmp_func key_cmp,
    const hash_table_key_hash_func key_hash
) {
    memset(ht, 0, sizeof(hash_table));
    ht->index_size = size;

    size_t index_size = size * sizeof(linked_list);
    ht->index = malloc(index_size);
    if (ht->index == NULL) {
        log_perror("malloc() failed");
        return false;
    }

    memset(ht->index, 0, index_size);

    ht->entry_size = 0;

    ht->key_cmp = key_cmp == NULL ? value_cmp_string : key_cmp;
    ht->key_hash = key_hash == NULL ? default_key_hash : key_hash;

    return true;
}

bool hash_table_rehash(hash_table* ht, const uint32_t new_size) {
    linked_list** old_index = ht->index;
    const size_t old_index_size = ht->index_size;

    size_t index_size = new_size * sizeof(linked_list);
    ht->index = malloc(index_size);
    if (ht->index == NULL) {
        log_perror("malloc() failed");
        return false;
    }

    memset(ht->index, 0, index_size);
    ht->index_size = new_size;

    // Rebuild index
    for (int i = 0; i < old_index_size; ++i) {
        const linked_list* p_list = old_index[i];
        if (p_list != NULL) {
            const list_node* p_iter = p_list->head;
            if (p_iter != NULL) {
                do {
                    hash_table_entry* p_entry = p_iter->value;
                    if (!hash_table_set_entry(ht, p_entry)) {
                        log_error("hash table is corrupt: hash_table_set_entry() failed during index rebuild");
                        return false;
                    }
                    p_iter = p_iter->next;
                }
                while (p_iter != NULL);
            }
        }
    }

    free(old_index);

    return true;
}

hash_table_entry* hash_table_init_entry(
    const void* key, size_t key_size,
    const void* value, size_t value_size
) {
    hash_table_entry* p_entry = malloc(sizeof(hash_table_entry));
    if (p_entry == NULL) {
        log_perror("malloc() failed for entry");
        return nullptr;
    }

    p_entry->must_destroy = 1;

    p_entry->key = malloc(key_size);
    if (p_entry->key == NULL) {
        log_perror("malloc() failed for key");
        free(p_entry);
        return nullptr;
    }

    p_entry->value = malloc(value_size);
    if (p_entry->value == NULL) {
        log_perror("malloc() failed for value");
        free(p_entry->key);
        free(p_entry);
        return nullptr;
    }

    memcpy(p_entry->key, key, key_size);
    memcpy(p_entry->value, value, value_size);

    return p_entry;
}

bool hash_table_destroy_entry(hash_table_entry* entry) {
    free(entry->key);
    entry->key = NULL;

    free(entry->value);
    entry->value = NULL;

    free((void *)entry);

    return true;
}

bool hash_table_set(hash_table* ht, void* key, void* value) {
    hash_table_entry *p_entry = hash_table_get_entry(ht, key);

    if (p_entry != NULL) {
        // Update existing entry
        p_entry->value = value;
        return true;
    }

    // Create a new entry
    p_entry = malloc(sizeof(hash_table_entry));
    if (p_entry == NULL) {
        log_perror("malloc() failed");
        return false;
    }

    memset(p_entry, 0, sizeof(hash_table_entry));
    p_entry->key = key;
    p_entry->value = value;

    return hash_table_set_entry(ht, p_entry);
}

bool hash_table_set_entry(hash_table* ht, hash_table_entry* entry) {
    if (ht->index == NULL) {
        log_error("hash table not initialized");
        return false;
    }

    const size_t index = find_index(ht, entry->key);
    linked_list* p_list = *(ht->index + index);
    if (p_list == NULL) {
        // First entry: Start a new linked list
        p_list = *(ht->index + index) = malloc(sizeof(linked_list));
        if (!linked_list_init(p_list)) {
            free(p_list);
            return false;
        }
    }
    else {
        list_node* p_curr = p_list->head;
        do {
            hash_table_entry* p_curr_ent = p_curr->value;
            if ((*ht->key_cmp)(p_curr_ent->key, entry->key) == 0) {
                // Update existing value
                p_curr_ent->value = entry->value;
                free(entry);
                return true;
            }

            p_curr = p_curr->next;
        }
        while (p_curr != NULL);
    }

    ++ht->entry_size;

    // Add to list
    return linked_list_push_tail(*(ht->index + index), entry);
}

hash_table_entry* hash_table_get_entry(const hash_table* ht, const void* key) {
    if (ht->index == NULL) {
        log_error("hash table not initialized");
        return nullptr;
    }

    const size_t index = find_index(ht, key);
    const linked_list* p_list = ht->index[index];
    if (p_list == NULL || p_list->size == 0) {
        // No entry
        return nullptr;
    }

    list_node* p_curr = p_list->head;
    do {
        hash_table_entry* p_entry = p_curr->value;
        if ((*ht->key_cmp)(p_entry->key, key) == 0) {
            return p_entry;
        }

        p_curr = p_curr->next;
    }
    while (p_curr != NULL);

    // No entry
    return nullptr;
}

void* hash_table_get(const hash_table* ht, const void* key) {
    hash_table_entry *entry = hash_table_get_entry(ht, key);
    if (entry == NULL) {
        return NULL;
    }

    return entry->value;
}

bool hash_table_del(hash_table* ht, const void* key) {
    if (ht->index == NULL) {
        log_error("hash table not initialized");
        return false;
    }

    size_t index = find_index(ht, key);
    linked_list* p_list = ht->index[index];
    if (p_list == NULL || p_list->size == 0) {
        log_debug("hash table has no entry at index %zu", index);
        return false;
    }

    int i = 0;
    size_t deleted_count = 0;
    list_node* p_curr = p_list->head;
    while (p_curr != NULL) {
        hash_table_entry* p_entry = p_curr->value;
        if ((*ht->key_cmp)(p_entry->key, key) == 0) {
            if (!linked_list_del_at(p_list, i)) {
                log_error("linked_list_del_at() failed during delete");
                return false;
            }
            ++deleted_count;
        }
        else {
            ++i;
        }

        p_curr = p_curr->next;
    };

    ht->entry_size -= deleted_count;

    return deleted_count > 0;
}

bool hash_table_iter(
    hash_table* ht,
    const hash_table_iter_func iter_func,
    void* iter_func_user_arg
) {
    if (ht->index == NULL) {
        log_error("hash table not initialized");
        return false;
    }

    if (ht->entry_size == 0) {
        // Empty
        return true;
    }

    for (int i = 0; i < ht->index_size; ++i) {
        const linked_list* p_list = ht->index[i];
        if (p_list != NULL) {
            const list_node* p_iter = p_list->head;
            if (p_iter != NULL) {
                do {
                    hash_table_entry* p_entry = p_iter->value;
                    p_iter = p_iter->next;
                    iter_func(p_entry, i, iter_func_user_arg);
                }
                while (p_iter != NULL);
            }
        }
    }

    return true;
}

/**
 * Iterator callback function that builds an array of keys
 *
 * @param[in] entry Iterated hash table entry
 * @param[in] _index Iteration index
 * @param[in,out] user_arg ht_array_builder_arg accumulator for iterated keys
 */
static void keys_iter_func(
    hash_table_entry* entry,
    const size_t _index,
    void* user_arg
) {
    struct hash_table_array_builder_arg* arg = user_arg;

    arg->items[arg->index++] = entry->key;
}

size_t hash_table_keys(hash_table* ht, void** keys) {
    struct hash_table_array_builder_arg user_arg;

    memset(&user_arg, 0, sizeof(user_arg));
    user_arg.items = keys;

    if (!hash_table_iter(ht, keys_iter_func, &user_arg)) {
        return -1;
    }

    return user_arg.index;
}

size_t hash_table_size(const hash_table* ht) {
    return ht->entry_size;
}

/**
 * Iterator callback function that builds an array of values
 *
 * @param[in] entry Iterated hash table entry
 * @param[in] _index Iteration index
 * @param[in,out] user_arg ht_array_builder_arg accumulator for iterated values
 */
static void values_iter_func(
    hash_table_entry* entry,
    const size_t _index,
    void* user_arg
) {
    struct hash_table_array_builder_arg* arg = user_arg;

    arg->items[arg->index++] = entry->value;
}

size_t hash_table_values(hash_table* ht, void** values) {
    struct hash_table_array_builder_arg user_arg;

    memset(&user_arg, 0, sizeof(user_arg));
    user_arg.items = values;

    if (!hash_table_iter(ht, values_iter_func, &user_arg)) {
        return -1;
    }

    return user_arg.index;
}

/**
 * Iterator callback function that destroys all entries
 *
 * @param[in] entry Iterated hash table entry
 * @param[in] _index Iteration index (ignored)
 * @param _user_arg Ignored
 */
static void destroy_iter_func(
    hash_table_entry* entry,
    const size_t _index,
    void* _user_arg
) {
    if (entry->must_destroy) {
        hash_table_destroy_entry(entry);
        return;
    }

    free((void *)entry);
}

bool hash_table_destroy(hash_table* ht) {
    if (ht->index == NULL) {
        return false;
    }

    if (!hash_table_iter(ht, destroy_iter_func, NULL)) {
        return false;
    }

    free(ht->index);
    ht->index = nullptr;

    ht->entry_size = 0;

    return true;
}
