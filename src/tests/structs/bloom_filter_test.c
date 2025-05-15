#include "bloom_filter_test.h"
#include "../../structs/bloom_filter.h"

CU_TestInfo* get_bloom_filter_tests() {
    static CU_TestInfo tests[] = {
        {"test_bloom_filter_init_and_destroy", test_bloom_filter_init_and_destroy},
        {"test_bloom_filter", test_bloom_filter},
        CU_TEST_INFO_NULL,
    };

    return tests;
}

void test_bloom_filter_init_and_destroy() {
    bloom_filter bf;
    CU_ASSERT_EQUAL(bloom_filter_init(&bf, 9), true)
    CU_ASSERT_PTR_NOT_NULL(bf.bit_array)
    CU_ASSERT_EQUAL(bf.bit_array->size_bits, 32) // Rounded to 32 bits
    CU_ASSERT_EQUAL(bf.hash_count, 2)
    CU_ASSERT_PTR_NOT_NULL(bf.bit_array)

    CU_ASSERT_EQUAL(bloom_filter_destroy(&bf), true)
    CU_ASSERT_EQUAL(bf.hash_count, 0)
    CU_ASSERT_PTR_NULL(bf.bit_array)
}

void test_bloom_filter() {
    bloom_filter bf;
    uint32_t bit_array_slot = 0;
    CU_ASSERT_EQUAL(bloom_filter_init(&bf, 32), true)
    CU_ASSERT_EQUAL(bit_array_slot, bf.bit_array->bit_array[0])

    CU_ASSERT_EQUAL(bloom_filter_check(&bf, (uint8_t *)"foo", 3), false)
    CU_ASSERT_EQUAL(bloom_filter_add(&bf, (uint8_t *)"foo", 3), true) // ["foo"]
    CU_ASSERT_EQUAL(bloom_filter_check(&bf, (uint8_t *)"foo", 3), true)
    CU_ASSERT_NOT_EQUAL(bit_array_slot, bf.bit_array->bit_array[0]) // Ensure bit array changed
    bit_array_slot = bf.bit_array->bit_array[0];

    CU_ASSERT_EQUAL(bloom_filter_check(&bf, (uint8_t *)"bar", 3), false)
    CU_ASSERT_EQUAL(bloom_filter_add(&bf, (uint8_t *)"bar", 3), true) // ["foo", "bar"]
    CU_ASSERT_EQUAL(bloom_filter_check(&bf, (uint8_t *)"bar", 3), true)
    CU_ASSERT_NOT_EQUAL(bit_array_slot, bf.bit_array->bit_array[0]) // Ensure bit array changed
    bit_array_slot = bf.bit_array->bit_array[0];

    CU_ASSERT_EQUAL(bloom_filter_check(&bf, (uint8_t *)"spangle", 7), false)
    CU_ASSERT_EQUAL(bloom_filter_add(&bf, (uint8_t *)"spangle", 7), true) // ["foo", "bar", "spangle"]
    CU_ASSERT_EQUAL(bloom_filter_check(&bf, (uint8_t *)"spangle", 7), true)
    CU_ASSERT_NOT_EQUAL(bit_array_slot, bf.bit_array->bit_array[0]) // Ensure bit array changed

    CU_ASSERT_EQUAL(bloom_filter_destroy(&bf), true)
}
