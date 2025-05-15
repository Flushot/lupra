#include <stdlib.h>

#include "hash_table_test.h"
#include "../../structs/hash_table.h"

CU_TestInfo* get_hash_table_tests() {
    static CU_TestInfo tests[] = {
        {"test_hash_table_init_and_destroy", test_hash_table_init_and_destroy},
        {"test_hash_table_rehash", test_hash_table_rehash},
        {"test_hash_table_get_and_set", test_hash_table_get_and_set},
        {"test_hash_table_set_entry", test_hash_table_set_entry},
        {"test_hash_table_del", test_hash_table_del},
        {"test_hash_table_keys", test_hash_table_keys},
        {"test_hash_table_values", test_hash_table_values},
        {"test_hash_table_has_no_duplicates", test_hash_table_has_no_duplicates},
        {"test_hash_table_iter", test_hash_table_iter},
        {"test_hash_table_size", test_hash_table_size},
        CU_TEST_INFO_NULL,
    };

    return tests;
}

void test_hash_table_init_and_destroy() {
    hash_table ht;
    CU_ASSERT_EQUAL(hash_table_init(&ht, 50, nullptr, nullptr), true)
    CU_ASSERT_EQUAL(ht.index_size, 50)

    CU_ASSERT_EQUAL(hash_table_set(&ht, "foo", "one"), true) // {"foo": "one"}
    CU_ASSERT_EQUAL(hash_table_set(&ht, "bar", "two"), true) // {"foo": "one", "bar": "two"}

    CU_ASSERT_EQUAL(hash_table_destroy(&ht), true)

    // Get non-existent values (will print warnings)
    CU_ASSERT_PTR_NULL(hash_table_get(&ht, "foo"))
    CU_ASSERT_PTR_NULL(hash_table_get(&ht, "bar"))
}

void test_hash_table_rehash() {
    hash_table ht;
    CU_ASSERT_EQUAL(hash_table_init(&ht, 2, nullptr, nullptr), true)
    CU_ASSERT_EQUAL(ht.index_size, 2)

    CU_ASSERT_EQUAL(hash_table_set(&ht, "foo", "one"), true) // {"foo": "one"}
    CU_ASSERT_STRING_EQUAL(hash_table_get(&ht, "foo"), "one")

    // Rehash
    CU_ASSERT_EQUAL(hash_table_rehash(&ht, 3), true)
    CU_ASSERT_EQUAL(ht.index_size, 3)
    CU_ASSERT_STRING_EQUAL(hash_table_get(&ht, "foo"), "one")

    CU_ASSERT_EQUAL(hash_table_destroy(&ht), true)
}

void test_hash_table_get_and_set() {
    hash_table ht;
    CU_ASSERT_EQUAL(hash_table_init(&ht, 50, nullptr, nullptr), true)

    CU_ASSERT_EQUAL(hash_table_set(&ht, "foo", "one"), true) // {"foo": "one"}
    CU_ASSERT_STRING_EQUAL(hash_table_get(&ht, "foo"), "one")
    CU_ASSERT_EQUAL(hash_table_set(&ht, "foo", "two"), true) // {"foo": "two"}
    CU_ASSERT_STRING_EQUAL(hash_table_get(&ht, "foo"), "two")
    CU_ASSERT_PTR_NULL(hash_table_get(&ht, "doesnt_exist"))

    CU_ASSERT_EQUAL(hash_table_set(&ht, "bar", "three"), true) // {"foo": "two", "bar": "three"}
    CU_ASSERT_STRING_EQUAL(hash_table_get(&ht, "foo"), "two")
    CU_ASSERT_STRING_EQUAL(hash_table_get(&ht, "bar"), "three")

    CU_ASSERT_EQUAL(hash_table_set(&ht, "spangle", "fez"), true) // {"foo": "two", "bar": "three", "spangle": "fez"}
    CU_ASSERT_STRING_EQUAL(hash_table_get(&ht, "foo"), "two")
    CU_ASSERT_STRING_EQUAL(hash_table_get(&ht, "bar"), "three")
    CU_ASSERT_STRING_EQUAL(hash_table_get(&ht, "spangle"), "fez")

    CU_ASSERT_EQUAL(hash_table_destroy(&ht), true)
}

void test_hash_table_set_entry() {
    int key = 3;
    int value = 6;
    hash_table ht;
    CU_ASSERT_EQUAL(hash_table_init(&ht, 50, nullptr, nullptr), true)

    hash_table_entry* entry = hash_table_init_entry(&key, sizeof(int), &value, sizeof(int));
    CU_ASSERT_PTR_NOT_NULL_FATAL(entry)
    CU_ASSERT_EQUAL(*((int *)entry->key), key)
    CU_ASSERT_EQUAL(*((int *)entry->value), value)

    CU_ASSERT_EQUAL(hash_table_set_entry(&ht, entry), true)

    CU_ASSERT_EQUAL(hash_table_destroy(&ht), true)
}

void test_hash_table_del() {
    hash_table ht;
    CU_ASSERT_EQUAL(hash_table_init(&ht, 50, nullptr, nullptr), true)

    CU_ASSERT_EQUAL(hash_table_set(&ht, "foo", "one"), true) // {"foo": "one"}
    CU_ASSERT_EQUAL(hash_table_set(&ht, "bar", "two"), true) // {"foo": "one", "bar": "two"}

    CU_ASSERT_EQUAL(hash_table_del(&ht, "bar"), true) // {"foo": "one"}
    CU_ASSERT_STRING_EQUAL(hash_table_get(&ht, "foo"), "one")
    CU_ASSERT_PTR_NULL(hash_table_get(&ht, "bar"))

    CU_ASSERT_EQUAL(hash_table_destroy(&ht), true)
}

void test_hash_table_keys() {
    hash_table ht;
    char* keys[5];
    memset(keys, 0, sizeof(keys));
    CU_ASSERT_EQUAL(hash_table_init(&ht, 50, nullptr, nullptr), true)

    CU_ASSERT_EQUAL(hash_table_keys(&ht, (void *)keys), 0)
    CU_ASSERT_PTR_NULL(keys[0])

    CU_ASSERT_EQUAL(hash_table_set(&ht, "foo", "one"), true) // {"foo": "one"}
    CU_ASSERT_EQUAL(hash_table_set(&ht, "bar", "two"), true) // {"foo": "one", "bar": "two"}
    CU_ASSERT_EQUAL(hash_table_set(&ht, "spangle", "three"), true) // {"foo": "one", "bar": "two", "spangle": "three"}

    CU_ASSERT_EQUAL(hash_table_keys(&ht, (void *)keys), 3)
    CU_ASSERT_STRING_EQUAL(keys[0], "bar")
    CU_ASSERT_STRING_EQUAL(keys[1], "spangle")
    CU_ASSERT_STRING_EQUAL(keys[2], "foo")
    CU_ASSERT_PTR_NULL(keys[3])

    CU_ASSERT_EQUAL(hash_table_destroy(&ht), true)
}

void test_hash_table_values() {
    hash_table ht;
    char* values[5];
    memset(values, 0, sizeof(values));
    CU_ASSERT_EQUAL(hash_table_init(&ht, 50, nullptr, nullptr), true)

    CU_ASSERT_EQUAL(hash_table_values(&ht, (void *)values), 0)
    CU_ASSERT_PTR_NULL(values[0])

    CU_ASSERT_EQUAL(hash_table_set(&ht, "foo", "one"), true) // {"foo": "one"}
    CU_ASSERT_EQUAL(hash_table_set(&ht, "bar", "two"), true) // {"foo": "one", "bar": "two"}
    CU_ASSERT_EQUAL(hash_table_set(&ht, "spangle", "three"), true) // {"foo": "one", "bar": "two", "spangle": "three"}

    CU_ASSERT_EQUAL(hash_table_values(&ht, (void *)values), 3)
    CU_ASSERT_STRING_EQUAL(values[0], "two")
    CU_ASSERT_STRING_EQUAL(values[1], "three")
    CU_ASSERT_STRING_EQUAL(values[2], "one")
    CU_ASSERT_PTR_NULL(values[3])

    CU_ASSERT_EQUAL(hash_table_destroy(&ht), true)
}

void test_hash_table_has_no_duplicates() {
    void* keys[2];
    hash_table ht;
    CU_ASSERT_EQUAL(hash_table_init(&ht, 100, nullptr, nullptr), true)

    CU_ASSERT_EQUAL(hash_table_set(&ht, "a", "one"), true) // {"a": "one"}
    CU_ASSERT_EQUAL(hash_table_set(&ht, "a", "two"), true) // {"a": "two"}

    CU_ASSERT_EQUAL(hash_table_keys(&ht, keys), 1) // One only key
    CU_ASSERT_STRING_EQUAL(hash_table_get(&ht, "a"), "two") // Last value set

    CU_ASSERT_EQUAL(hash_table_destroy(&ht), true)
}

static void test_hash_table_iter_func(hash_table_entry* entry, const size_t index, void* result) {
    strcat(result, "(");
    strcat(result, entry->key);
    strcat(result, "=");
    strcat(result, entry->value);
    strcat(result, ")");
}

void test_hash_table_iter() {
    hash_table ht;
    char result[500];
    memset(result, 0, sizeof(result));
    CU_ASSERT_EQUAL(hash_table_init(&ht, 50, nullptr, nullptr), true)

    CU_ASSERT_EQUAL(hash_table_set(&ht, "foo", "one"), true) // {"foo": "one"}
    CU_ASSERT_EQUAL(hash_table_set(&ht, "bar", "two"), true) // {"foo": "one", "bar": "two"}

    CU_ASSERT_EQUAL(hash_table_iter(&ht, test_hash_table_iter_func, result), true)
    CU_ASSERT_STRING_EQUAL(result, "(bar=two)(foo=one)")

    CU_ASSERT_EQUAL(hash_table_destroy(&ht), true)
}

void test_hash_table_size() {
    hash_table ht;
    CU_ASSERT_EQUAL(hash_table_init(&ht, 50, nullptr, nullptr), true)
    CU_ASSERT_EQUAL(hash_table_size(&ht), 0)

    CU_ASSERT_EQUAL(hash_table_set(&ht, "foo", "one"), true) // {"foo": "one"}
    CU_ASSERT_EQUAL(hash_table_size(&ht), 1)

    CU_ASSERT_EQUAL(hash_table_set(&ht, "bar", "two"), true) // {"foo": "one", "bar": "two"}
    CU_ASSERT_EQUAL(hash_table_size(&ht), 2)

    CU_ASSERT_EQUAL(hash_table_destroy(&ht), true)
    CU_ASSERT_EQUAL(hash_table_size(&ht), 0)
}
