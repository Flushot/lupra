#include <stdio.h>

#include "bloom_filter.h"
#include "../algos/murmur3.h"
#include "../utils/log.h"

bool bloom_filter_init(bloom_filter* bf, const size_t size) {
    bf->hash_count = 2;

    bf->bit_array = malloc(sizeof(bit_array));
    if (bf->bit_array == NULL) {
        log_perror("malloc() failed");
        return false;
    }

    if (!bit_array_init(bf->bit_array, size)) {
        free(bf->bit_array);
        return false;
    }

    return true;
}

/**
 * Hash function that uses the Kirsch & Mitzenmacher technique
 * https://www.eecs.harvard.edu/~michaelm/postscripts/rsa2008.pdf
 *
 * Simulates k hash functions by combining murmur3 hashes
 *
 * @param[in] key Key to hash
 * @param[in] key_len Size of the key
 * @param[in] k Total number of hashes to generate
 * @param[in] m Modulo (total number of bits in bloom filter)
 * @param[out] hashes_out Generated hashes
 */
static void bloom_hashes(
    const uint8_t* key,
    const size_t key_len,
    const uint32_t k,
    const uint32_t m,
    uint32_t* hashes_out
) {
    const uint32_t hash1 = murmur3(key, key_len, 0x5f3759df); // Seed: Fast inverse sqrt const
    const uint32_t hash2 = murmur3(key, key_len, 0x9e3779b9); // Seed: Golden ratio prime

    for (uint32_t i = 0; i < k; ++i) {
        hashes_out[i] = (hash1 + i * hash2) % m;
    }
}

bool bloom_filter_add(const bloom_filter* bf, const uint8_t* key, const size_t key_len) {
    uint32_t hashes[bf->hash_count];
    bloom_hashes(key, key_len, bf->hash_count, bf->bit_array->size_bits, hashes);

    for (uint32_t i = 0; i < bf->hash_count; ++i) {
        bit_array_set(bf->bit_array, hashes[i]);
    }

    return true;
}

bool bloom_filter_check(const bloom_filter* bf, const uint8_t* key, const size_t key_len) {
    uint32_t hashes[bf->hash_count];
    bloom_hashes(key, key_len, bf->hash_count, bf->bit_array->size_bits, hashes);

    for (uint32_t i = 0; i < bf->hash_count; ++i) {
        if (!bit_array_test(bf->bit_array, hashes[i])) {
            return false;
        }
    }

    return true;
}

bool bloom_filter_destroy(bloom_filter* bf) {
    if (bf->bit_array != NULL) {
        if (!bit_array_destroy(bf->bit_array)) {
            return false;
        }

        free(bf->bit_array);
        bf->bit_array = nullptr;
    }

    bf->hash_count = 0;

    return true;
}
