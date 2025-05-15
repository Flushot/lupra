#pragma once

#include <CUnit/Basic.h>

CU_TestInfo* get_bloom_filter_tests();

void test_bloom_filter_init_and_destroy();

void test_bloom_filter();
