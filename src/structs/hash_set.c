#include "hash_set.h"

bool inline hash_set_init(
    hash_set* hs,
    const uint32_t size,
    const value_cmp_func key_cmp,
    const hash_table_key_hash_func key_hash
) {
    return hash_table_init(&hs->table, size, key_cmp, key_hash);
}

bool inline hash_set_rehash(hash_set* hs, uint32_t new_size) {
    return hash_table_rehash(&hs->table, new_size);
}

bool inline hash_set_destroy(hash_set* hs) {
    return hash_table_destroy(&hs->table);
}

bool inline hash_set_add(hash_set* hs, void* value) {
    return hash_table_set(&hs->table, value, nullptr);
}

bool inline hash_set_contains(hash_set* hs, void* value) {
    return hash_table_get_entry(&hs->table, value) != NULL;
}

bool inline hash_set_del(hash_set* hs, void* value) {
    return hash_table_del(&hs->table, value);
}

size_t inline hash_set_size(hash_set* hs) {
    return hash_table_size(&hs->table);
}
