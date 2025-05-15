#include "heap_test.h"
#include "../../structs/heap.h"
#include "../../structs/array_list.h"

//                     0  1  2  3  4  5  6  7  8   9
static int values[] = {0, 1, 2, 3, 4, 5, 7, 8, 69, 420};

CU_TestInfo* get_heap_tests() {
    static CU_TestInfo tests[] = {
        {"test_heap_init_and_destroy", test_heap_init_and_destroy},
        {"test_min_heap", test_min_heap},
        {"test_max_heap", test_max_heap},
        CU_TEST_INFO_NULL,
    };

    return tests;
}

/*
static void heap_dump(const heap* h) {
    printf("Heap: [");

    for (size_t i = 0; i < h->size; ++i) {
        const int* value = array_list_get_at(h->heap_array, i);
        if (value == NULL) {
            printf("NULL");
        }
        else {
            printf("%i", *value);
        }

        if (i < h->size - 1) {
            printf(", ");
        }
    }

    printf("]\n");
}
*/

void test_heap_init_and_destroy() {
    heap min_heap;
    CU_ASSERT_EQUAL(heap_init(&min_heap, MIN_HEAP, value_cmp_int, 5), true)
    CU_ASSERT_EQUAL(min_heap.type, MIN_HEAP)
    CU_ASSERT_EQUAL(min_heap.heap_array->capacity, 6); // Rounded up to nearest even
    CU_ASSERT_EQUAL(min_heap.size, 0)
    CU_ASSERT_EQUAL(heap_destroy(&min_heap), true)

    heap max_heap;
    CU_ASSERT_EQUAL(heap_init(&max_heap, MAX_HEAP, value_cmp_int, 100), true)
    CU_ASSERT_EQUAL(max_heap.size, 0)
    CU_ASSERT_EQUAL(max_heap.heap_array->capacity, 100);
    CU_ASSERT_EQUAL(heap_destroy(&max_heap), true)
}

void test_min_heap() {
    heap min_heap;
    CU_ASSERT_EQUAL(heap_init(&min_heap, MIN_HEAP, value_cmp_int, 7), true)

    CU_ASSERT_EQUAL(heap_push(&min_heap, &values[5]), true) // [5 | [5]]
    CU_ASSERT_EQUAL(heap_push(&min_heap, &values[3]), true) // [3 | [3, 5]]
    CU_ASSERT_EQUAL(heap_push(&min_heap, &values[8]), true) // [69 | [3, 5, 69]]
    CU_ASSERT_EQUAL(heap_push(&min_heap, &values[9]), true) // [420 | [3, 5, 69, 420]]
    CU_ASSERT_EQUAL(heap_push(&min_heap, &values[4]), true) // [4 | [3, 4, 5, 69, 420]]
    CU_ASSERT_EQUAL(heap_push(&min_heap, &values[4]), true) // [4 | [3, 4, 4, 5, 69, 420]]
    CU_ASSERT_EQUAL(heap_push(&min_heap, &values[1]), true) // [1 | [1, 3, 4, 4, 5, 69, 420]]
    CU_ASSERT_EQUAL(heap_push(&min_heap, &values[7]), true) // [8 | [1, 3, 4, 4, 5, 8, 69, 420]]
    CU_ASSERT_EQUAL(heap_push(&min_heap, &values[6]), true) // [7 | [1, 3, 4, 4, 5, 7, 8, 69, 420]]
    CU_ASSERT_EQUAL(min_heap.size, 9)

    CU_ASSERT_EQUAL(*(int *)heap_peek(&min_heap), 1);
    CU_ASSERT_EQUAL(*(int *)heap_pop(&min_heap), 1) // [1 | [3, 4, 4, 5, 7, 8, 69, 420]]
    CU_ASSERT_EQUAL(*(int *)heap_pop(&min_heap), 3) // [3 | [4, 4, 5, 7, 8, 69, 420]]
    CU_ASSERT_EQUAL(*(int *)heap_pop(&min_heap), 4) // [4 | [4, 5, 7, 8, 69, 420]]
    CU_ASSERT_EQUAL(*(int *)heap_pop(&min_heap), 4) // [4 | [5, 7, 8, 69, 420]]
    CU_ASSERT_EQUAL(*(int *)heap_pop(&min_heap), 5) // [5 | [7, 8, 69, 420]]
    CU_ASSERT_EQUAL(min_heap.size, 4)
    CU_ASSERT_EQUAL(*(int *)heap_pop(&min_heap), 7) // [7 | [8, 69, 420]]
    CU_ASSERT_EQUAL(*(int *)heap_pop(&min_heap), 8) // [8 | [69, 420]]
    CU_ASSERT_EQUAL(*(int *)heap_pop(&min_heap), 69) // [69 | [420]]
    CU_ASSERT_EQUAL(*(int *)heap_pop(&min_heap), 420) // [420 | []]
    CU_ASSERT_EQUAL(min_heap.size, 0)

    bool ret = heap_destroy(&min_heap);
    CU_ASSERT_EQUAL(ret, true)
}

void test_max_heap() {
    heap max_heap;
    CU_ASSERT_EQUAL(heap_init(&max_heap, MAX_HEAP, value_cmp_int, 9), true)

    CU_ASSERT_EQUAL(heap_push(&max_heap, &values[5]), true) // [5 | [5]]
    CU_ASSERT_EQUAL(heap_push(&max_heap, &values[3]), true) // [3 | [5, 3]]
    CU_ASSERT_EQUAL(heap_push(&max_heap, &values[8]), true) // [68 | [69, 5, 3]
    CU_ASSERT_EQUAL(heap_push(&max_heap, &values[9]), true) // [420 | [420, 69, 5, 3]]
    CU_ASSERT_EQUAL(heap_push(&max_heap, &values[4]), true) // [4 | [420, 69, 5, 4, 3]]
    CU_ASSERT_EQUAL(heap_push(&max_heap, &values[4]), true) // [4 | [420, 69, 5, 4, 4, 3]]
    CU_ASSERT_EQUAL(heap_push(&max_heap, &values[1]), true) // [1 | [420, 69, 5, 4, 4, 3, 1]]
    CU_ASSERT_EQUAL(heap_push(&max_heap, &values[7]), true) // [8 | [420, 69, 8, 5, 4, 4, 3, 1]]
    CU_ASSERT_EQUAL(heap_push(&max_heap, &values[6]), true) // [7 | [420, 69, 8, 7, 5, 4, 4, 3, 1]]
    CU_ASSERT_EQUAL(max_heap.size, 9)

    CU_ASSERT_EQUAL(*(int *)heap_peek(&max_heap), 420);
    CU_ASSERT_EQUAL(*(int *)heap_pop(&max_heap), 420) // [420 | [69, 8, 7, 5, 4, 4, 3, 1]]
    CU_ASSERT_EQUAL(*(int *)heap_pop(&max_heap), 69) // [69 | [8, 7, 5, 4, 4, 3, 1]]
    CU_ASSERT_EQUAL(*(int *)heap_pop(&max_heap), 8) // [8 | [7, 5, 4, 4, 3, 1]]
    CU_ASSERT_EQUAL(*(int *)heap_pop(&max_heap), 7) // [7 | [5, 4, 4, 3, 1]]
    CU_ASSERT_EQUAL(*(int *)heap_pop(&max_heap), 5) // [5 | [4, 4, 3, 1]]
    CU_ASSERT_EQUAL(max_heap.size, 4)
    CU_ASSERT_EQUAL(*(int *)heap_pop(&max_heap), 4) // [4 | [4, 3, 1]]
    CU_ASSERT_EQUAL(*(int *)heap_pop(&max_heap), 4) // [4 | [3, 1]]
    CU_ASSERT_EQUAL(*(int *)heap_pop(&max_heap), 3) // [3 | [1]]
    CU_ASSERT_EQUAL(*(int *)heap_pop(&max_heap), 1) // [1 | []]
    CU_ASSERT_EQUAL(max_heap.size, 0)

    CU_ASSERT_EQUAL(heap_destroy(&max_heap), true)
}
