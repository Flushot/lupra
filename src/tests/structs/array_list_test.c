#include <stdlib.h>

#include "array_list_test.h"
#include "../../structs/array_list.h"

static int values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

CU_TestInfo* get_array_list_tests() {
    static CU_TestInfo tests[] = {
        {"test_array_list_init_and_destroy", test_array_list_init_and_destroy},
        {"test_array_list", test_array_list},
        {"test_array_list_iter", test_array_list_iter},
        {"test_array_list_set_at_beyond_capacity", test_array_list_set_at_beyond_capacity},
        CU_TEST_INFO_NULL,
    };

    return tests;
}

/*
static void array_list_dump(const array_list* lst) {
    printf("ArrayList: [");

    for (size_t i = 0; i < lst->size; ++i) {
        const int *value = lst->array[i];
        if (value == NULL) {
            printf("NULL");
        } else {
            printf("%i", *value);
        }

        if (i < lst->size - 1) {
            printf(", ");
        }
    }

    printf("]\n");
}
*/

void test_array_list_init_and_destroy() {
    array_list lst;
    const size_t capacity = 3;
    CU_ASSERT_EQUAL(array_list_init(&lst, capacity), true)
    CU_ASSERT_PTR_NOT_NULL(lst.array)
    CU_ASSERT_EQUAL(lst.size, 0)
    CU_ASSERT_EQUAL(lst.capacity, capacity)

    CU_ASSERT_EQUAL(array_list_destroy(&lst), true)
    CU_ASSERT_PTR_NULL(lst.array)
    CU_ASSERT_EQUAL(lst.size, 0)
    CU_ASSERT_EQUAL(lst.capacity, 0)
}

void test_array_list() {
    array_list lst;
    CU_ASSERT_EQUAL(array_list_init(&lst, 3), true) // []

    CU_ASSERT_EQUAL(array_list_push_tail(&lst, &values[5]), true) // [5]
    CU_ASSERT_EQUAL(array_list_push_tail(&lst, &values[7]), true) // [5, 7]
    CU_ASSERT_EQUAL(array_list_push_tail(&lst, &values[9]), true) // [5, 7, 9]
    CU_ASSERT_EQUAL(lst.size, 3)

    CU_ASSERT_EQUAL(array_list_index_of(&lst, &values[7]), 1)
    CU_ASSERT_EQUAL(array_list_index_of(&lst, &values[11]), -1)

    CU_ASSERT_EQUAL(array_list_binary_search_index_of(&lst, &values[7]), 1)
    CU_ASSERT_EQUAL(array_list_binary_search_index_of(&lst, &values[11]), -1)

    CU_ASSERT_EQUAL(*(int *)array_list_get_at(&lst, 2), 9)
    CU_ASSERT_EQUAL(*(int *)array_list_del_at(&lst, 1), 7) // [5, 9]
    CU_ASSERT_EQUAL(lst.size, 2)

    CU_ASSERT_EQUAL(array_list_push_tail(&lst, &values[11]), true) // [5, 9, 11]
    CU_ASSERT_EQUAL(*(int *)array_list_del_at(&lst, 1), 9) // [5, 11]
    CU_ASSERT_PTR_NULL(array_list_del_at(&lst, 9))
    CU_ASSERT_EQUAL(*(int *)array_list_pop_head(&lst), 5) // [11]
    CU_ASSERT_EQUAL(*(int *)array_list_pop_head(&lst), 11) // []
    CU_ASSERT_EQUAL(lst.size, 0)

    CU_ASSERT_EQUAL(array_list_push_head(&lst, &values[5]), true) // [5]
    CU_ASSERT_EQUAL(array_list_push_head(&lst, &values[7]), true) // [7, 5]
    CU_ASSERT_EQUAL(array_list_push_head(&lst, &values[9]), true) // [9, 7, 5]

    CU_ASSERT_EQUAL(*(int *)array_list_get_at(&lst, 2), 5)
    CU_ASSERT_EQUAL(*(int *)array_list_get_at(&lst, 0), 9)
    CU_ASSERT_EQUAL(*(int *)array_list_del_value(&lst, &values[9]), 9) // [7, 5]
    CU_ASSERT_EQUAL(lst.size, 2)
    CU_ASSERT_EQUAL(*(int *)array_list_get_at(&lst, 0), 7)

    CU_ASSERT_EQUAL(array_list_destroy(&lst), true)
}

void test_array_list_set_at_beyond_capacity() {
    array_list lst;
    CU_ASSERT_EQUAL(array_list_init(&lst, 3), true) // []

    CU_ASSERT_EQUAL(array_list_set_at(&lst, 0, &values[5]), true) // [5]
    CU_ASSERT_EQUAL(lst.size, 1)
    CU_ASSERT_EQUAL(*(int *)array_list_get_at(&lst, 0), 5)

    CU_ASSERT_EQUAL(array_list_set_at(&lst, 2, &values[2]), true) // [5, NULL, 2], Skipped one
    CU_ASSERT_EQUAL(lst.size, 3)
    CU_ASSERT_EQUAL(*(int *)array_list_get_at(&lst, 2), 2)
    CU_ASSERT_EQUAL(array_list_get_at(&lst, 1), NULL) // Backfilled with NULL

    CU_ASSERT_EQUAL(array_list_set_at(&lst, 3, &values[3]), true) // [5, NULL, 2, 3], Beyond capacity
    CU_ASSERT_EQUAL(lst.size, 4)
    CU_ASSERT_EQUAL(lst.capacity, 4) // Capacity increased

    CU_ASSERT_EQUAL(array_list_set_at(&lst, 4, &values[4]), true) // [5, NULL, 2, 3, 4], Beyond capacity
    CU_ASSERT_EQUAL(lst.size, 5)
    CU_ASSERT_EQUAL(lst.capacity, 5) // Capacity increased

    CU_ASSERT_EQUAL(*(int *)array_list_get_at(&lst, 0), 5)
    CU_ASSERT_EQUAL(array_list_get_at(&lst, 1), NULL)
    CU_ASSERT_EQUAL(*(int *)array_list_get_at(&lst, 2), 2)
    CU_ASSERT_EQUAL(*(int *)array_list_get_at(&lst, 3), 3)
    CU_ASSERT_EQUAL(*(int *)array_list_get_at(&lst, 4), 4)

    CU_ASSERT_EQUAL(array_list_destroy(&lst), true)
}

static void test_array_list_iter_func(void* value, const size_t index, void* result) {
    strcat(result, "(");

    char index_str[3];
    snprintf((char *)&index_str, sizeof(index_str), "%zu", index);
    strcat(result, index_str);

    strcat(result, "=");

    char value_str[5];
    snprintf((char *)&value_str, sizeof(value_str), "%i", *(int*)value);
    strcat(result, value_str);

    strcat(result, ")");
}

void test_array_list_iter() {
    array_list lst;
    char result[500];
    memset(result, 0, sizeof(result));
    CU_ASSERT_EQUAL(array_list_init(&lst, 3), true) // []

    CU_ASSERT_EQUAL(array_list_push_tail(&lst, &values[5]), true) // [5]
    CU_ASSERT_EQUAL(array_list_push_tail(&lst, &values[7]), true) // [5, 7]

    array_list_iter(&lst, test_array_list_iter_func, result);
    CU_ASSERT_STRING_EQUAL(result, "(0=5)(1=7)")

    CU_ASSERT_EQUAL(array_list_destroy(&lst), true)
}
