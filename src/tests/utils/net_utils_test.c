#include "net_utils_test.h"
#include "../../utils/net_utils.h"

CU_TestInfo* get_net_utils_tests() {
    static CU_TestInfo tests[] = {
        {"test_net_utils_ipv4_to_long", test_net_utils_ipv4_to_long},
        {"test_net_utils_long_to_ipv4", test_net_utils_long_to_ipv4},
        {"test_net_utils_ipv4_matches", test_net_utils_ipv4_matches},
        {"test_net_utils_ether_ntoa", test_net_utils_ether_ntoa},
        CU_TEST_INFO_NULL,
    };

    return tests;
}

void test_net_utils_ipv4_to_long() {
    CU_ASSERT_EQUAL(net_utils_ipv4_to_long("0.0.0.0"), 0)
    CU_ASSERT_EQUAL(net_utils_ipv4_to_long("1.2.3.4"), 16909060)
    CU_ASSERT_EQUAL(net_utils_ipv4_to_long("255.255.255.255"), 4294967295)
    CU_ASSERT_EQUAL(net_utils_ipv4_to_long(""), -1)
}

void test_net_utils_long_to_ipv4() {
    char result[16];

    net_utils_long_to_ipv4(0, (char *)&result);
    CU_ASSERT_STRING_EQUAL(result, "0.0.0.0")

    net_utils_long_to_ipv4(16909060, (char *)&result);
    CU_ASSERT_STRING_EQUAL(result, "1.2.3.4")

    net_utils_long_to_ipv4(4294967295, (char *)&result);
    CU_ASSERT_STRING_EQUAL(result, "255.255.255.255")
}

void test_net_utils_ipv4_matches() {
    uint32_t result;

    result = net_utils_ipv4_matches("1.2.3.4", "1.2.3.4", 32);
    CU_ASSERT_EQUAL(result, 1);

    result = net_utils_ipv4_matches("1.2.3.4", "1.2.3.100", 32);
    CU_ASSERT_EQUAL(result, 0);

    result = net_utils_ipv4_matches("1.2.3.4", "1.2.3.100", 24);
    CU_ASSERT_EQUAL(result, 1);

    result = net_utils_ipv4_matches("1.2.3.4", "1.2.100.100", 24);
    CU_ASSERT_EQUAL(result, 0);

    result = net_utils_ipv4_matches("1.2.3.4", "1.2.100.100", 16);
    CU_ASSERT_EQUAL(result, 1);

    result = net_utils_ipv4_matches("1.2.3.4", "1.100.100.100", 16);
    CU_ASSERT_EQUAL(result, 0);

    result = net_utils_ipv4_matches("1.2.3.4", "1.100.100.100", 8);
    CU_ASSERT_EQUAL(result, 1);

    result = net_utils_ipv4_matches("1.2.3.4", "100.100.100.100", 0);
    CU_ASSERT_EQUAL(result, 1);
}

void test_net_utils_ether_ntoa() {
    const uint8_t* input = (uint8_t *)"\xab\x57\xd8\x36\xda\x88";

    const char* result = net_utils_ether_ntoa(input);
    CU_ASSERT_STRING_EQUAL(result, "ab:57:d8:36:da:88")
}
