#pragma once

/**
 * Doubly-linked list node
 *
 * @relates linked_list
 */
typedef struct linked_list_node {
    void* value;
    struct linked_list_node *prev, *next;
} list_node;

/**
 * A linked list is a non-contiguous list of elements allocated on the heap.
 *
 * Linked lists are useful when you want low-cost updates/deletions and usually only sequentially scanning items.
 * They aren't very efficient at random access or when you need a contiguous block of memory (like arrays) though.
 *
 * Linked lists are doubly-linked and can efficiently be used as a queue or a stack.
 *
 * **Example**
 * ```c
 * linked_list lst;
 * linked_list_init(&lst);
 *
 * int in_value = 1;
 * linked_list_push_tail(&lst, &in_value); // Append [1] to list
 *
 * int *out_value = linked_list_tail(&lst); // Get last item
 *
 * assert(*in_value == *out_value);
 *
 * linked_list_destroy(&lst);
 * ```
 */
typedef struct linked_list {
    list_node *head, *tail;
    size_t size;
} linked_list;

/**
 * Initialize the linked list
 *
 * Time complexity: O(1)
 *
 * @relates linked_list
 * @param[out] lst Empty list to initialize
 * @return true on success, false on failure
 */
bool linked_list_init(linked_list* lst);

/**
 * Insert a value into the list at position
 *
 * Time complexity: O(1)
 *
 * @relates linked_list
 * @param[in,out] lst List
 * @param[in] value Value to insert
 * @param[in] pos Position to insert at
 * @return true on success, false on failure
 */
bool linked_list_insert_at(linked_list* lst, void* value, size_t pos);

/**
 * Get value at position in the list
 *
 * Time complexity: O(1)
 *
 * @relates linked_list
 * @param[in] lst List
 * @param[in] pos Position to get item at
 * @return Value of the item (or NULL it not found)
 */
void* linked_list_get_at(const linked_list* lst, size_t pos);

/**
 * Delete value at position in the list
 *
 * Time complexity: O(1)
 *
 * @relates linked_list
 * @param[in,out] lst List
 * @param[in] pos Position to delete at
 * @return true on success, false on failure
 */
bool linked_list_del_at(linked_list* lst, size_t pos);

/**
 * Get the first (head) value from the list
 *
 * Time complexity: O(1)
 *
 * @relates linked_list
 * @param[in] lst List
 * @return Value of head (or NULL if empty)
 */
void* linked_list_head(const linked_list* lst);

/**
 * Push value to the head of the list (prepend)
 *
 * Time complexity: O(1)
 *
 * @relates linked_list
 * @param[in,out] lst List
 * @param[in] value Value
 * @return true on success, false on failure
 */
bool linked_list_push_head(linked_list* lst, void* value);

/**
 * Pop value from the head of the list
 *
 * Time complexity: O(1)
 *
 * @relates linked_list
 * @param[in,out] lst List
 * @return Value that was popped (or NULL if not found)
 */
void* linked_list_pop_head(linked_list* lst);

/**
 * Get the last value in the list
 *
 * Time complexity: O(1)
 *
 * @relates linked_list
 * @param[in] lst List
 * @return Value of tail (or NULL if empty)
 */
void* linked_list_tail(const linked_list* lst);

/**
 * Push value to the tail of the list (append)
 *
 * Time complexity: O(1)
 *
 * @relates linked_list
 * @param[in,out] lst List
 * @param[in] value Value
 * @return true on success, false on failure
 */
bool linked_list_push_tail(linked_list* lst, void* value);

/**
 * Pop the last (tail) value from the list
 *
 * Time complexity: O(1)
 *
 * @relates linked_list
 * @param[in,out] lst List
 * @return Value from tail (or NULL if empty)
 */
void* linked_list_pop_tail(linked_list* lst);

/**
 * List iterator callback function
 *
 * @relates linked_list
 * @param[in] lst Iterated list node
 * @param[in] index Iteration index
 * @param user_arg Optional user arg
 */
typedef void (*list_iter_func)(
    const list_node* lst,
    size_t index,
    void* user_arg
);

/**
 * Iterate list items, forwards
 *
 * Time complexity: O(n)
 *
 * @relates linked_list
 * @param[in] lst List
 * @param[in] iter_func Iterator callback function
 * @param iter_func_user_arg Optional argument to pass to callback function
 * @return true on success, false on failure
 */
void linked_list_forward_iter(
    const linked_list* lst,
    list_iter_func iter_func,
    void* iter_func_user_arg
);

/**
 * Iterate list items, backwards
 *
 * Time complexity: O(n)
 *
 * @relates linked_list
 * @param[in] lst List
 * @param[in] iter_func Iterator callback function
 * @param iter_func_user_arg Optional argument to pass to callback function
 * @return true on success, false on failure
 */
void linked_list_backward_iter(
    const linked_list* lst,
    list_iter_func iter_func,
    void* iter_func_user_arg
);

/**
 * Destroy the list
 *
 * Time complexity: O(n)
 *
 * @relates linked_list
 * @param[in,out] lst List
 * @return true on success, false on failure
 */
bool linked_list_destroy(linked_list* lst);
