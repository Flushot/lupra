#pragma once

#include <CUnit/Basic.h>

CU_TestInfo* get_linked_list_tests();

void test_linked_list_init_and_destroy();

void test_linked_list();

void test_linked_list_forward_iter();

void test_linked_list_backward_iter();
