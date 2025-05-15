#include <stdlib.h>
#include <CUnit/Basic.h>

#include "library.h"
#include "tests/algos/array_test.h"
#include "tests/algos/murmur3_test.h"
#include "tests/structs/linked_list_test.h"
#include "tests/structs/array_list_test.h"
#include "tests/structs/hash_table_test.h"
#include "tests/structs/bit_array_test.h"
#include "tests/structs/bloom_filter_test.h"
#include "tests/structs/heap_test.h"
#include "tests/utils/net_utils_test.h"

static int suite_setup() {
    srand(0);

    if (!lupra_init()) {
        return CUE_SINIT_FAILED;
    }

    return CUE_SUCCESS;
}

static int suite_teardown() {
    if (!lupra_destroy()) {
        return CUE_SCLEAN_FAILED;
    }

    return CUE_SUCCESS;
}

int main(int argc, char** argv) {
    // Initialize the CUnit test registry
    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }

    CU_SuiteInfo suites[] = {
        {"array", suite_setup, suite_teardown, NULL, NULL, get_array_tests()},
        {"murmur3", suite_setup, suite_teardown, NULL, NULL, get_murmur3_tests()},
        {"linked_list", suite_setup, suite_teardown, NULL, NULL, get_linked_list_tests()},
        {"array_list", suite_setup, suite_teardown, NULL, NULL, get_array_list_tests()},
        {"hash_table", suite_setup, suite_teardown, NULL, NULL, get_hash_table_tests()},
        {"bit_array", suite_setup, suite_teardown, NULL, NULL, get_bit_array_tests()},
        {"bloom_filter", suite_setup, suite_teardown, NULL, NULL, get_bloom_filter_tests()},
        {"heap", suite_setup, suite_teardown, NULL, NULL, get_heap_tests()},
        {"net_utils", suite_setup, suite_teardown, NULL, NULL, get_net_utils_tests()},
        CU_SUITE_INFO_NULL,
    };

    if (CU_register_suites(suites) != CUE_SUCCESS) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Run all tests using the CUnit Basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    const unsigned int total_failures = CU_get_number_of_failures();

    CU_cleanup_registry();

    const CU_ErrorCode error_code = CU_get_error();
    if (error_code != CUE_SUCCESS) {
        return error_code;
    }

    if (total_failures > 0) {
        return 1;
    }

    return CUE_SUCCESS;
}
