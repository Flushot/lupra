#include "bit_array_test.h"
#include "../../structs/bit_array.h"

CU_TestInfo* get_bit_array_tests() {
    static CU_TestInfo tests[] = {
        {"test_bit_array_init_and_destroy", test_bit_array_init_and_destroy},
        {"test_bit_array", test_bit_array},
        CU_TEST_INFO_NULL,
    };

    return tests;
}

void test_bit_array_init_and_destroy() {
    bit_array ba;
    CU_ASSERT_EQUAL(bit_array_init(&ba, 30), true)
    CU_ASSERT_EQUAL(ba.size_bits, 32)
    CU_ASSERT_PTR_NOT_NULL(ba.bit_array)

    CU_ASSERT_EQUAL(bit_array_destroy(&ba), true)
    CU_ASSERT_EQUAL(ba.size_bits, 0)
    CU_ASSERT_PTR_NULL(ba.bit_array)
}

void test_bit_array() {
    bit_array ba;
    CU_ASSERT_EQUAL(bit_array_init(&ba, 5), true)

    CU_ASSERT_EQUAL(bit_array_test(&ba, 1), false)
    CU_ASSERT_EQUAL(bit_array_test(&ba, 16), false)

    bit_array_set(&ba, 1);
    CU_ASSERT_EQUAL(bit_array_test(&ba, 1), true)
    CU_ASSERT_EQUAL(bit_array_test(&ba, 16), false)

    bit_array_set(&ba, 16);
    CU_ASSERT_EQUAL(bit_array_test(&ba, 1), true)
    CU_ASSERT_EQUAL(bit_array_test(&ba, 16), true)

    bit_array_clear(&ba, 16);
    CU_ASSERT_EQUAL(bit_array_test(&ba, 1), true)
    CU_ASSERT_EQUAL(bit_array_test(&ba, 16), false)

    CU_ASSERT_EQUAL(bit_array_destroy(&ba), true)
}
