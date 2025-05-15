#pragma once

#include <CUnit/Basic.h>

CU_TestInfo* get_net_utils_tests();

void test_net_utils_ipv4_to_long();

void test_net_utils_long_to_ipv4();

void test_net_utils_ipv4_matches();

void test_net_utils_ether_ntoa();
