#include <stdio.h>
#include <stdlib.h>

#include "linked_list.h"
#include "../utils/log.h"

/**
 * Create a new list node
 *
 * Time complexity: O(1)
 *
 * @param[in] value Value of node
 * @return List node
 */
static list_node* make_node(void* value) {
    list_node* item = malloc(sizeof(list_node));
    if (item == NULL) {
        log_perror("malloc() failed");
        return nullptr;
    }

    item->value = value;
    item->prev = nullptr;
    item->next = nullptr;

    return item;
}

/**
 * Find a list node at a given position
 *
 * This needs to traverse the entire list, so it's not very efficient for
 * large lists
 *
 * Time complexity: O(n)
 *
 * @param[in] list List
 * @param[in] pos Position
 * @return List node (or NULL if not found)
 */
static list_node* find_node_at(list_node* list, const size_t pos) {
    list_node* p_iter = list;

    for (int i = 0; p_iter != NULL; ++i) {
        if (i == pos) {
            return p_iter;
        }

        p_iter = p_iter->next;
    }

    return nullptr;
}

bool linked_list_init(linked_list* lst) {
    lst->head = nullptr;
    lst->tail = nullptr;
    lst->size = 0;

    return true;
}

bool linked_list_insert_at(linked_list* lst, void* value, const size_t pos) {
    list_node* p_head = lst->head;
    list_node* p_existing = find_node_at(p_head, pos);

    if (p_existing == NULL) {
        log_error("node does not exist at index %zu", pos);
        return false;
    }

    list_node* p_node = make_node(value);
    if (p_node == NULL) {
        return false;
    }

    if (p_head == NULL) {
        lst->head = p_node;
    }

    p_node->prev = p_existing->prev;
    p_node->next = p_existing;

    if (p_node->next == NULL) {
        lst->tail = p_node;
    }

    ++lst->size;

    return true;
}

void* linked_list_get_at(const linked_list* lst, const size_t pos) {
    list_node* p_node = find_node_at(lst->head, pos);

    if (p_node == NULL) {
        return NULL;
    }

    return p_node->value;
}

bool linked_list_del_at(linked_list* lst, const size_t pos) {
    if (lst->head == NULL) {
        log_error("list is empty");
        return false;
    }

    list_node* p_node = find_node_at(lst->head, pos);
    if (p_node == NULL) {
        log_error("node does not exist at index %zu", pos);
        return false;
    }

    list_node* p_before = p_node->prev;
    list_node* p_after = p_node->next;

    if (p_before != NULL) {
        p_before->next = p_after;
    }
    else {
        lst->head = p_after;
    }

    if (p_after != NULL) {
        p_after->prev = p_before;
    }
    else {
        lst->tail = p_before;
    }

    p_node->prev = nullptr;
    p_node->next = nullptr;

    free(p_node);
    --lst->size;

    return true;
}

void* linked_list_head(const linked_list* lst) {
    if (lst->head == NULL) {
        return NULL;
    }

    return lst->head->value;
}

bool linked_list_push_head(linked_list* lst, void* value) {
    list_node* p_head = lst->head;

    list_node* p_node = make_node(value);
    if (p_node == NULL) {
        return false;
    }

    if (p_head != NULL) {
        lst->head = p_node;
        p_node->next = p_head;
        p_head->prev = p_node;
    }
    else {
        lst->head = p_node;
    }

    if (p_node->next == NULL) {
        lst->tail = p_node;
    }

    ++lst->size;

    return true;
}

void* linked_list_pop_head(linked_list* lst) {
    list_node* p_node = lst->head;

    if (p_node == NULL) {
        return NULL;
    }

    void* value = p_node->value;
    lst->head = p_node->next;
    p_node->next = nullptr;

    free(p_node);
    --lst->size;

    return value;
}

void* linked_list_tail(const linked_list* lst) {
    if (lst->tail == NULL) {
        return NULL;
    }

    return lst->tail->value;
}

bool linked_list_push_tail(linked_list* lst, void* value) {
    list_node* p_tail = lst->tail;

    list_node* p_node = make_node(value);
    if (p_node == NULL) {
        return false;
    }

    if (p_tail != NULL) {
        p_tail->next = p_node;
        p_node->prev = p_tail;
    }
    else {
        lst->head = p_node;
    }

    lst->tail = p_node;

    ++lst->size;

    return true;
}

void* linked_list_pop_tail(linked_list* lst) {
    list_node* p_node = lst->tail;

    if (p_node == NULL) {
        return NULL;
    }

    list_node* p_before = p_node->prev;

    lst->tail = p_before;

    if (p_before == NULL) {
        // List is now empty
        lst->head = nullptr;
    }
    else {
        lst->tail = p_before;
        p_before->next = nullptr;
        p_node->prev = nullptr;
    }

    void* value = p_node->value;

    free(p_node);
    --lst->size;

    return value;
}

void linked_list_forward_iter(
    const linked_list* lst,
    const list_iter_func iter_func,
    void* iter_func_user_arg
) {
    list_node* p_node = lst->head;
    if (p_node == NULL) {
        // Empty list (noop)
        return;
    }

    int i = 0;
    do {
        list_node* p_next = p_node->next;
        iter_func(p_node, i++, iter_func_user_arg);
        p_node = p_next;
    }
    while (p_node != NULL);
}

void linked_list_backward_iter(
    const linked_list* lst,
    const list_iter_func iter_func,
    void* iter_func_user_arg
) {
    list_node* p_node = lst->tail;
    if (p_node == NULL) {
        // Empty list (noop)
        return;
    }

    int i = 0;
    do {
        list_node* p_prev = p_node->prev;
        iter_func(p_node, i++, iter_func_user_arg);
        p_node = p_prev;
    }
    while (p_node != NULL);
}

/**
 * Iterator callback function that destroys all list nodes
 *
 * @param[in] item Iterated list node
 * @param[in] _index Iteration index
 * @param _user_arg Ignored
 */
static void destroy_iter_func(
    const list_node* item,
    size_t _index,
    void* _user_arg
) {
    free((void *)item);
}

bool linked_list_destroy(linked_list* lst) {
    linked_list_forward_iter(lst, destroy_iter_func, NULL);

    lst->head = nullptr;
    lst->tail = nullptr;
    lst->size = 0;

    return true;
}
