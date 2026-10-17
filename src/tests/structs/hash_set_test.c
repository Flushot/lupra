#include "hash_set_test.h"
#include "../../structs/hash_set.h"

CU_TestInfo* get_hash_set_tests() {
    static CU_TestInfo tests[] = {
        {"test_hash_set_init_and_destroy", test_hash_set_init_and_destroy},
        {"test_hash_set", test_hash_set},
        {"test_hash_set_rehash", test_hash_set_rehash},
        CU_TEST_INFO_NULL,
    };

    return tests;
}

void test_hash_set_init_and_destroy() {
    hash_set hs;

    CU_ASSERT_TRUE(hash_set_init(&hs, 5, nullptr, nullptr));
    CU_ASSERT_EQUAL(hash_set_size(&hs), 0)
    CU_ASSERT_TRUE(hash_set_destroy(&hs))
}

void test_hash_set() {
    hash_set hs;

    CU_ASSERT_TRUE(hash_set_init(&hs, 5, nullptr, nullptr))
    CU_ASSERT_EQUAL(hash_set_size(&hs), 0)
    CU_ASSERT_FALSE(hash_set_contains(&hs, "foo"))
    CU_ASSERT_FALSE(hash_set_contains(&hs, "bar"))
    CU_ASSERT_FALSE(hash_set_contains(&hs, "spangle"))

    CU_ASSERT_TRUE(hash_set_add(&hs, "foo"))
    CU_ASSERT_EQUAL(hash_set_size(&hs), 1)
    CU_ASSERT_TRUE(hash_set_contains(&hs, "foo"))
    CU_ASSERT_FALSE(hash_set_contains(&hs, "bar"))
    CU_ASSERT_FALSE(hash_set_contains(&hs, "spangle"))

    CU_ASSERT_TRUE(hash_set_add(&hs, "bar"))
    CU_ASSERT_EQUAL(hash_set_size(&hs), 2)
    CU_ASSERT_TRUE(hash_set_contains(&hs, "foo"))
    CU_ASSERT_TRUE(hash_set_contains(&hs, "bar"))
    CU_ASSERT_FALSE(hash_set_contains(&hs, "spangle"))

    CU_ASSERT_TRUE(hash_set_del(&hs, "foo"))
    CU_ASSERT_EQUAL(hash_set_size(&hs), 1)
    CU_ASSERT_FALSE(hash_set_contains(&hs, "foo"))
    CU_ASSERT_TRUE(hash_set_contains(&hs, "bar"))
    CU_ASSERT_FALSE(hash_set_contains(&hs, "spangle"))

    CU_ASSERT_TRUE(hash_set_del(&hs, "bar"))
    CU_ASSERT_EQUAL(hash_set_size(&hs), 0)
    CU_ASSERT_FALSE(hash_set_contains(&hs, "foo"))
    CU_ASSERT_FALSE(hash_set_contains(&hs, "bar"))
    CU_ASSERT_FALSE(hash_set_contains(&hs, "spangle"))

    CU_ASSERT_TRUE(hash_set_destroy(&hs))
}

void test_hash_set_rehash() {
    hash_set hs;
    CU_ASSERT_TRUE(hash_set_init(&hs, 2, nullptr, nullptr))
    CU_ASSERT_EQUAL(hs.table.index_size, 2)
    CU_ASSERT_FALSE(hash_set_contains(&hs, "foo"))

    CU_ASSERT_TRUE(hash_set_add(&hs, "foo"))
    CU_ASSERT_TRUE(hash_set_contains(&hs, "foo"))

    // Rehash
    CU_ASSERT_TRUE(hash_set_rehash(&hs, 3))
    CU_ASSERT_EQUAL(hs.table.index_size, 3)
    CU_ASSERT_TRUE(hash_set_contains(&hs, "foo"))

    CU_ASSERT_TRUE(hash_set_destroy(&hs))
}
