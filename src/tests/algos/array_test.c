#include "array_test.h"

#include "../../algos/array.h"

CU_TestInfo* get_array_tests() {
    static CU_TestInfo tests[] = {
        {"test_array_binary_search", test_array_binary_search},
        CU_TEST_INFO_NULL,
    };

    return tests;
}

void test_array_binary_search() {
    const int arr[] = {1, 2, 3, 4, 5, 6};

    size_t retval = array_binary_search(int, arr, 6, 4);
    CU_ASSERT_EQUAL(retval, 3)

    retval = array_binary_search(int, arr, 6, 10);
    CU_ASSERT_EQUAL(retval, -1)
}
