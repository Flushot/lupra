#pragma once

#include "array_list.h"
#include "../utils/value.h"

/**
 * Type of heap
 *
 * @relates heap
 */
enum heap_type {
    MIN_HEAP,
    MAX_HEAP
};

/**
 * A heap is an ordered binary tree that keeps either the largest value (max heap) or the smallest value
 * (min heap) at its root.
 *
 * Heaps are useful when you always need to know the largest or smallest value of some set of items, and are commonly
 * used as priority queues.
 *
 * Values in the heap are stored in a flat array, which allows for efficient memory access.
 *
 * **Example**
 * ```c
 * heap min_heap;
 * heap_init(&min_heap, MIN_HEAP, value_cmp_int, 5);
 *
 * int one = 1;
 * int two = 2;
 * int three = 3;
 *
 * heap_push(&min_heap, &two);
 * assert(*(int *)heap_peek(&min_heap) == 2);
 *
 * heap_push(&min_heap, &three);
 * assert(*(int *)heap_peek(&min_heap) == 2);
 *
 * heap_push(&min_heap, &one);
 * assert(*(int *)heap_peek(&min_heap) == 1);
 *
 * heap_destroy(&heap);
 * ```
*/
typedef struct heap {
    enum heap_type type;
    size_t size;
    array_list* heap_array;
    value_cmp_func value_cmp;
} heap;

/**
 * Initialize the heap
 *
 * @relates heap
 * @param h Heap to initialize
 * @param type Type of heap
 * @param value_cmp
 * @param capacity
 * @return true on success, false on failure
 */
bool heap_init(
    heap* h,
    enum heap_type type,
    value_cmp_func value_cmp,
    size_t capacity
);

/**
 * Push a value onto the heap
 *
 * Time complexity: O(lg n)
 *
 * @relates heap
 * @param h Heap
 * @param value Value to push
 * @return true on success, false on failure
 */
bool heap_push(heap* h, void *value);

/**
 * Pop a value from the heap
 *
 * Time complexity: O(lg n)
 *
 * @relates heap
 * @param h Heap
 * @return Popped value or NULL if failed
 */
void* heap_pop(heap* h);

/**
 * Peek at the top value in the heap
 *
 * Time complexity: O(1)
 *
 * @relates heap
 * @param h Heap
 * @return Value at top of heap or NULL if empty
 */
void* heap_peek(const heap* h);

/**
 * Destroy the heap
 *
 * Time complexity: O(1)
 *
 * @relates heap
 * @param h Heap to destroy
 * @return true on success, false on failure
 */
bool heap_destroy(heap* h);
