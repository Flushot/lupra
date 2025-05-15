#pragma once

#include <CUnit/Basic.h>

CU_TestInfo* get_array_list_tests();

void test_array_list_init_and_destroy();

void test_array_list();

void test_array_list_set_at_beyond_capacity();

void test_array_list_iter();
