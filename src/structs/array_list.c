#include <stdio.h>

#include "array_list.h"
#include "../algos/array.h"
#include "../utils/log.h"

/**
 * Allocate new array in array list
 *
 * Time complexity: O(1) if within capacity, otherwise O(n)
 *
 * @param[in,out] lst Array list
 * @return true on success, false on failure
 */
static bool alloc_array(array_list* lst) {
    void* new_array;

    if (lst->array == NULL) {
        // First allocation
        new_array = malloc(lst->capacity * sizeof(void*));
    }
    else {
        // Subsequent allocation
        new_array = realloc(lst->array, lst->capacity * sizeof(void*));
    }

    if (new_array == NULL) {
        log_perror("malloc() failed");
        return false;
    }

    lst->array = new_array;

    return true;
}

bool array_list_init(array_list* lst, const size_t capacity) {
    lst->size = 0;
    lst->capacity = capacity;
    lst->array = nullptr;

    if (!alloc_array(lst)) {
        return false;
    }

    // Zero the array
    memset(lst->array, 0, lst->capacity * sizeof(void*));

    return true;
}

size_t array_list_index_of(const array_list* lst, const void* value) {
    for (size_t i = 0; i < lst->size; ++i) {
        if (lst->array[i] == value) {
            return i;
        }
    }

    return -1;
}

size_t inline array_list_binary_search_index_of(const array_list* lst, const void* value) {
    return array_binary_search(void*, lst->array, lst->size, value);
}

/**
 * Remove item at pos and shift items after it left
 *
 * Time complexity: O(n)
 *
 * @param[in,out] lst List
 * @param[in] pos Position to remove
 */
static void shift_left(array_list* lst, const size_t pos) {
    --lst->size;

    for (size_t i = pos; i < lst->size; ++i) {
        lst->array[i] = lst->array[i + 1];
    }
}

/**
 * Remove item at pos and shift items before it right
 *
 * Time complexity: O(n)
 *
 * @param[in,out] lst List
 * @param[in] pos Position to remove
 * @return true on success, false on failure
 */
static bool shift_right(array_list* lst, const size_t pos) {
    if ((pos + 1) >= lst->capacity) {
        if (!array_list_resize(lst, pos + 1)) {
            return false;
        }
    }

    for (size_t i = lst->size; i > pos; --i) {
        lst->array[i] = lst->array[i - 1];
    }

    return true;
}

bool array_list_insert_at(array_list* lst, void* value, const size_t pos) {
    if (pos > lst->size) {
        log_error("index %zu is out of bounds %zu", pos, lst->size);
        return false;
    }

    if (lst->size == 0 && pos == 0) {
        lst->array[0] = value;
        ++lst->size;
        return true;
    }

    if (!shift_right(lst, pos)) {
        return false;
    }

    lst->array[pos] = value;
    ++lst->size;

    return true;
}

inline void* array_list_get_at(const array_list* lst, const size_t pos) {
    if (pos >= lst->size) {
        // log_debug("list position %zu is beyond size (upper: %zu)", pos, lst->size - 1);
        return NULL;
    }

    return lst->array[pos];
}

inline bool array_list_set_at(array_list* lst, const size_t pos, void* value) {
    if (pos >= lst->size) {
        if (pos >= lst->capacity) {
            if (!array_list_resize(lst, pos + 1)) {
                return false;
            }
        }

        // Fill in between values with NULLs
        for (size_t i = lst->size; i < pos; ++i) {
            lst->array[i] = NULL;
        }

        lst->size = pos + 1;
    }

    lst->array[pos] = value;

    return true;
}

void* array_list_del_at(array_list* lst, const size_t pos) {
    if (pos >= lst->size) {
        return NULL;
    }

    void* value = array_list_get_at(lst, pos);

    shift_left(lst, pos);

    return value;
}

void* array_list_del_value(array_list* lst, const void* value) {
    const size_t index = array_list_index_of(lst, value);
    if (index == -1) {
        return NULL;
    }

    return array_list_del_at(lst, index);
}

void* array_list_head(const array_list* lst) {
    if (lst->size == 0) {
        return NULL;
    }

    return lst->array[0];
}

void* array_list_tail(const array_list* lst) {
    if (lst->size == 0) {
        return NULL;
    }

    return lst->array[lst->size - 1];
}

bool array_list_push_head(array_list* lst, void* value) {
    return array_list_insert_at(lst, value, 0);
}

void* array_list_pop_head(array_list* lst) {
    if (lst->size == 0) {
        return NULL;
    }

    return array_list_del_at(lst, 0);
}

bool array_list_push_tail(array_list* lst, void* value) {
    return array_list_insert_at(lst, value, lst->size);
}

void* array_list_pop_tail(array_list* lst) {
    if (lst->size == 0) {
        return NULL;
    }

    return array_list_del_at(lst, lst->size - 1);
}

void array_list_iter(
    const array_list* lst,
    const array_list_iter_func iter_func,
    void* iter_func_user_arg
) {
    for (size_t i = 0; i < lst->size; ++i) {
        iter_func(lst->array[i], i, iter_func_user_arg);
    }
}

bool array_list_resize(array_list* lst, const size_t capacity) {
    if (capacity == lst->capacity) {
        log_debug("resize ignored: new capacity %zu is the same as existing capacity", capacity);
        return true;
    }

    if (capacity < lst->capacity && capacity < lst->size) {
        log_error("resize failed: new capacity %zu is smaller than list size %zu", capacity, lst->size);
        return false;
    }

    const size_t old_capacity = lst->capacity;

    lst->capacity = capacity;

    if (!alloc_array(lst)) {
        lst->capacity = old_capacity;
        return false;
    }

    // Zero the newly allocated portion of the array
    //memset(lst->array + old_capacity, 0, (capacity - old_capacity - 1) * sizeof(lst->value_size));

    return true;
}

bool array_list_destroy(array_list* lst) {
    if (lst->array != NULL) {
        free(lst->array);
        lst->array = nullptr;
    }

    lst->size = 0;
    lst->capacity = 0;

    return true;
}
