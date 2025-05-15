#pragma once

/**
 * An array list (also commonly known as a dynamic array) is an array that can dynamically grow as items are added.
 *
 * Array lists are useful anywhere you need an array (for low-cost random access reads/append/overwrites with less
 * frequent prepends and insertions), but don't want to deal with the manual book keeping around resizing it when the
 * items being added exceed its capacity.
 *
 * The way C normally allocates arrays is using a fixed-size contiguous block of memory. This means if you want
 * to add more elements after the array has reached its capacity, you'll typically need to allocate a new array,
 * copy all the items over, and destroy the old array. Array list basically does all this for you (with some
 * optimizations).
 *
 * The way this works is that the array list has a maximum `capacity` that's set on initialization (or specified
 * when you manually resize it). When the number of items inserted into the array exceeds this `capacity`, the
 * array is automatically resized to accommodate.
 *
 * **Example**
 * ```c
 * array_list lst;
 * array_list_init(&lst, 5); // List of ints with capacity of 5
 *
 * int in_value = 1;
 * array_list_push_tail(&lst, &in_value); // Append [1] to list
 *
 * int* out_value = array_list_get_at(&lst, 0); // Get item at position 0
 *
 * assert(*in_value == *out_value);
 *
 * array_list_destroy(&lst);
 * ```
 */
typedef struct array_list {
    size_t size;
    size_t capacity;
    void** array;
} array_list;

/**
 * Initialize array list
 *
 * Time complexity: O(1)
 *
 * @relates array_list
 * @param[out] lst Empty list to initialize
 * @param[in] capacity Max capacity of the array list (before it gets resized)
 * @return true on success, false on failure
 */
bool array_list_init(array_list* lst, size_t capacity);

/**
 * Find index of value in the array list
 *
 * This uses a linear search and works with any type of comparable value.
 *
 * Time complexity: O(n)
 *
 * @relates array_list
 * @param[in] lst Array list
 * @param[in] value Value to search
 * @return Index or -1 if not found
 */
size_t array_list_index_of(const array_list* lst, const void* value);

/**
 * Find index of value in the array list
 *
 * This uses a binary search and only works with numeric values (that can be
 * compared by inequality operators) over an already sorted array list.
 *
 * Time complexity: O(lg n)
 *
 * @relates array_list
 * @param lst Sorted array list
 * @param value Value to search for
 * @return Index or -1 if not found
 */
size_t array_list_binary_search_index_of(const array_list* lst, const void* value);

/**
 * Insert a value into the array list at position
 *
 * Time complexity: O(1)
 *
 * @relates array_list
 * @param[in,out] lst Array list
 * @param[in] value Value to insert
 * @param[in] pos Position to insert at
 * @return true on success, false on failure
 */
bool array_list_insert_at(array_list* lst, void* value, size_t pos);

/**
 * Get value at position in the array list
 *
 * Time complexity: O(1)
 *
 * @relates array_list
 * @param[in] lst Array list
 * @param[in] pos Position to get item at
 * @return Value of the item (or NULL it not found)
 */
void* array_list_get_at(const array_list* lst, size_t pos);

/**
 * Set value at position in the array list
 *
 * Time complexity: O(1) if within size, otherwise O(n)
 *
 * @relates array_list
 * @param lst Array list
 * @param pos Position to set item at
 * @param value Value of the item
 * @return true on success, false on failure
 */
bool array_list_set_at(array_list* lst, size_t pos, void* value);

/**
 * Delete value at position in the array list
 *
 * Time complexity: O(n)
 *
 * @relates array_list
 * @param[in,out] lst Array list
 * @param[in] pos Position to delete at
 * @return Value at position (or NULL if not found)
 */
void* array_list_del_at(array_list* lst, size_t pos);

/**
 * Get the first item in the array list
 *
 * Time complexity: O(1)
 *
 * @relates array_list
 * @param lst Array list
 * @return Head value
 */
void* array_list_head(const array_list* lst);

/**
 * Get the last item in the array list
 *
 * Time complexity: O(1)
 *
 * @relates array_list
 * @param lst Array list
 * @return Tail value
 */
void* array_list_tail(const array_list* lst);

/**
 * Delete value from the array list
 *
 * Time complexity: O(n)
 *
 * @relates array_list
 * @param[in,out] lst Array list
 * @param[in] value Value to delete
 * @return Deleted value (or NULL if not found)
 */
void* array_list_del_value(array_list* lst, const void* value);

/**
 * Push value to the head of the array list (prepend)
 *
 * Time complexity: O(n)
 *
 * @relates array_list
 * @param[in,out] lst Array list
 * @param[in] value Value
 * @return true on success, false on failure
 */
bool array_list_push_head(array_list* lst, void* value);

/**
 * Pop value from the head of the array list
 *
 * Time complexity: O(n)
 *
 * @relates array_list
 * @param[in,out] lst Array list
 * @return Value that was popped (or NULL if not found)
 */
void* array_list_pop_head(array_list* lst);

/**
 * Push value to the tail of the array list (append)
 *
 * Time complexity: O(1) if within capacity, otherwise O(n)
 *
 * @relates array_list
 * @param[in,out] lst Array list
 * @param[in] value Value
 * @return true on success, false on failure
 */
bool array_list_push_tail(array_list* lst, void* value);

/**
 * Pop the last (tail) value from the array list
 *
 * Time complexity: O(1) if within capacity, otherwise O(n)
 *
 * @relates array_list
 * @param[in,out] lst Array list
 * @return Value from tail (or NULL if empty)
 */
void* array_list_pop_tail(array_list* lst);

/**
 * Array list iterator callback function
 *
 * @relates array_list
 * @param[in] value Iterated array list value
 * @param[in] index Iteration index
 * @param user_arg Optional user arg
 */
typedef void (*array_list_iter_func)(
    void* value,
    size_t index,
    void* user_arg
);

/**
 * Iterate array list values
 *
 * Time complexity: O(n)
 *
 * @relates array_list
 * @param[in] lst Array list
 * @param[in] iter_func Iterator callback function
 * @param iter_func_user_arg Optional argument to pass to callback function
 */
void array_list_iter(
    const array_list* lst,
    array_list_iter_func iter_func,
    void* iter_func_user_arg
);

/**
 * Resize the array list
 *
 * Time complexity: O(1) if the new size is contiguous, otherwise O(n)
 *
 * @relates array_list
 * @param[in,out] lst Array list
 * @param[in] capacity New capacity
 * @return true on success, false on failure
 */
bool array_list_resize(array_list* lst, size_t capacity);

/**
 * Destroy the array list
 *
 * Time complexity: O(1)
 *
 * @relates array_list
 * @param[in,out] lst Array list
 * @return true on success, false on failure
 */
bool array_list_destroy(array_list* lst);
