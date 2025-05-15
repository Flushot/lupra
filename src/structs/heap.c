#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "heap.h"
#include "../utils/log.h"

bool heap_init(
    heap* h,
    const enum heap_type type,
    const value_cmp_func value_cmp,
    const size_t capacity
) {
    h->type = type;
    h->value_cmp = value_cmp;
    h->size = 0;

    h->heap_array = malloc(sizeof(array_list));
    if (h->heap_array == NULL) {
        log_perror("malloc() failed");
        return false;
    }

    // Ensure capacity is even
    size_t actual_capacity = capacity;
    if (actual_capacity % 2 != 0) {
        actual_capacity += 1;
    }

    if (!array_list_init(h->heap_array, actual_capacity)) {
        free(h->heap_array);
        h->heap_array = nullptr;
        return false;
    }

    return true;
}

/**
 * Get the parent of a binary tree node from an array
 *
 * Time complexity: O(1)
 *
 * @param index Node index
 * @return Parent node index
 */
static inline size_t parent(const size_t index) {
    return (size_t)floor((double)(index - 1) / 2);
}

/**
 * Get the left child of a binary tree node from an array
 *
 * Time complexity: O(1)
 *
 * @param index Node index
 * @return Left child node index
 */
static inline size_t left_child(const size_t index) {
    return 2 * index + 1;
}

/**
 * Get the right child of a binary tree node from an array
 *
 * Time complexity: O(1)
 *
 * @param index Node index
 * @return Right child node index
 */
static inline size_t right_child(const size_t index) {
    return 2 * index + 2;
}

/**
 * Push value up heap (e.g., tail to head)
 *
 * Time complexity: O(lg n)
 *
 * @param h Heap
 * @param index Starting point
 * @return true on success, false on failure
 */
static bool heapify_up(heap* h, const size_t index) {
    if (index == 0) {
        return true;
    }

    const size_t parent_idx = parent(index);
    if (parent_idx < 0) {
        return true;
    }

    void* parent_val = array_list_get_at(h->heap_array, parent_idx);
    void* val = array_list_get_at(h->heap_array, index);

    const int cmp_result = h->value_cmp(parent_val, val);

    if (
        (h->type == MIN_HEAP && cmp_result > 0 /* parent_val > val */) ||
        (h->type == MAX_HEAP && cmp_result <= 0 /* parent_val <= val */)
    ) {
        // Swap value with parent
        if (
            !array_list_set_at(h->heap_array, parent_idx, val) ||
            !array_list_set_at(h->heap_array, index, parent_val)
        ) {
            log_error("failed to swap value with parent");
            return false;
        }

        return heapify_up(h, parent_idx);
    }

    return true;
}

/**
 * Push value down heap (e.g., head to tail)
 *
 * Time complexity: O(lg n)
 *
 * @param h Heap
 * @param index Starting point
 * @return true on success, false on failure
 */
static bool heapify_down(heap* h, const size_t index) {
    const size_t left_idx = left_child(index);
    const size_t right_idx = right_child(index);

    if (index >= h->size || left_idx >= h->size) {
        return true;
    }

    void* left_val = array_list_get_at(h->heap_array, left_idx);
    void* right_val = array_list_get_at(h->heap_array, right_idx);
    void* val = array_list_get_at(h->heap_array, index);

    const int lv_rv_cmp_result = h->value_cmp(left_val, right_val);

    if (h->type == MIN_HEAP) {
        if (
            // left_val > val > right_val
            lv_rv_cmp_result > 0 /* left_val > right_val */ &&
            h->value_cmp(val, right_val) > 0 /* val > right_val */
        ) {
            // Swap value with the right so that it's smallest
            if (
                !array_list_set_at(h->heap_array, index, right_val) ||
                !array_list_set_at(h->heap_array, right_idx, val)
            ) {
                log_error("failed to swap value with right");
                return false;
            }

            return heapify_down(h, right_idx); // Continue down the right side
        }

        if (
            // left_val <= val <= right
            lv_rv_cmp_result <= 0 /* right_val >= left_val */ &&
            h->value_cmp(val, left_val) >= 0 /* val >= left_val */
        ) {
            // Swap value with the left so that it's smallest
            if (
                !array_list_set_at(h->heap_array, index, left_val) ||
                !array_list_set_at(h->heap_array, left_idx, val)
            ) {
                log_error("failed to swap value with left");
                return false;
            }

            return heapify_down(h, left_idx); // Continue down the left side
        }
    }

    if (h->type == MAX_HEAP) {
        // left_val < val < right_val
        // left_val >= val >= right

        if (
            // left_val < val < right_val
            lv_rv_cmp_result < 0 /* left_val < right_val */ &&
            h->value_cmp(val, right_val) < 0 /* val < right_val */
        ) {
            // Swap value with the right so that it's largest
            if (
                !array_list_set_at(h->heap_array, index, right_val) ||
                !array_list_set_at(h->heap_array, right_idx, val)
            ) {
                log_error("failed to swap value with right");
                return false;
            }

            return heapify_down(h, right_idx); // Continue down the right side
        }

        if (
            // left_val >= val => right
            lv_rv_cmp_result >= 0 /* right_val <= left_val */ &&
            h->value_cmp(val, left_val) <= 0 /* val <= left_val */
        ) {
            // Swap value with the left so that it's largest
            if (
                !array_list_set_at(h->heap_array, index, left_val) ||
                !array_list_set_at(h->heap_array, left_idx, val)
            ) {
                log_error("failed to swap value with left");
                return false;
            }

            return heapify_down(h, left_idx); // Continue down the left side
        }
    }

    return true;
}

bool heap_push(heap* h, void* value) {
    if (!array_list_set_at(h->heap_array, h->size, value)) {
        return false;
    }

    return heapify_up(h, h->size++);
}

void* heap_pop(heap* h) {
    if (h->size == 0) {
        log_error("heap is empty");
        return NULL;
    }

    // Pop head
    void* value = array_list_head(h->heap_array);

    --h->size;

    if (h->size == 0) {
        return value;
    }

    // Move tail to head
    void* tail_val = array_list_get_at(h->heap_array, h->size);
    array_list_set_at(h->heap_array, 0, tail_val);

    if (!heapify_down(h, 0)) {
        return NULL;
    }

    return value;
}

void* heap_peek(const heap *h) {
    if (h->size == 0) {
        log_error("heap is empty");
        return NULL;
    }

    return array_list_head(h->heap_array);
}

bool heap_destroy(heap* h) {
    if (!array_list_destroy(h->heap_array)) {
        return false;
    }

    if (h->heap_array != NULL) {
        free(h->heap_array);
        h->heap_array = nullptr;
    }

    return true;
}
