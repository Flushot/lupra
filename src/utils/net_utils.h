#pragma once

#include <stdint.h>

/**
 * Convert an IPv4 address string into a long
 *
 * @param[in] ip_addr IPv4 address string to convert to long
 * @return Long representation of IP address, or -1 if failed
 */
uint32_t net_utils_ipv4_to_long(const char* ip_addr);

/**
 * Convert a long into an IPv4 address string
 *
 * @param[in] long_addr Long to convert to IPv4 address string
 * @param[out] ip_addr_out Output buffer for IPv4 address string
 */
char* net_utils_long_to_ipv4(uint32_t long_addr, char* ip_addr_out);

/**
 * Matches IPv4 addresses using net_bits significant bits
 *
 * @param[in] test_ip_addr IPv4 address to test
 * @param[in] match_ip_addr Matching IPv4 address or network
 * @param[in] net_bits Number of network bits to use when matching against match_ip_addr (0-32)
 * @return true when matched, false when not matched
 */
bool net_utils_ipv4_matches(const char* test_ip_addr, const char* match_ip_addr, uint8_t net_bits);

/**
 * Convert ethernet MAC address to human-readable string
 *
 * Not thread-safe
 *
 * @param[in] ether_addr MAC address
 * @return Human-readable string
 */
char* net_utils_ether_ntoa(const uint8_t* ether_addr);
