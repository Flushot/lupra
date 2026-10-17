#pragma once

#include <CUnit/Basic.h>

CU_TestInfo* get_hash_set_tests();

void test_hash_set_init_and_destroy();

void test_hash_set();

void test_hash_set_rehash();
