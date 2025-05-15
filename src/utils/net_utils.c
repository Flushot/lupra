#include <stdio.h>
#include <stdlib.h>

#include "net_utils.h"
#include "log.h"

uint32_t net_utils_ipv4_to_long(const char* ip_addr) {
    uint32_t long_addr = 0;

    char* ip_buffer = strdup(ip_addr);
    if (ip_buffer == NULL) {
        log_perror("strdup() failed");
        return -1;
    }

    char* token = strtok(ip_buffer, ".");
    if (token == NULL) {
        return -1;
    }

    while (token != NULL) {
        long_addr <<= 8;
        long_addr |= atoi(token) & 0xFF;

        token = strtok(nullptr, "."); // get next token
    }

    free(ip_buffer);

    return long_addr;
}

char* net_utils_long_to_ipv4(const uint32_t long_addr, char* ip_addr_out) {
    char octet[4];

    ip_addr_out[0] = 0;

    for (uint8_t i = 0; i < 4; ++i) {
        const int ret = snprintf(
            (char *)&octet, sizeof(char) * 4, "%d",
            (long_addr >> (24 - 8 * i)) & 0xFF
        );

        if (ret < 0) {
            log_perror("snprintf() failed");
            return nullptr;
        }

        strcat(ip_addr_out, (char *)&octet);

        if (i < 3) {
            strcat(ip_addr_out, ".");
        }
    }

    return ip_addr_out;
}

bool net_utils_ipv4_matches(const char* test_ip_addr, const char* match_ip_addr, const uint8_t net_bits) {
    const uint32_t mask = ~0UL << (32 - net_bits);

    return (net_utils_ipv4_to_long(test_ip_addr) & mask) == (net_utils_ipv4_to_long(match_ip_addr) & mask);
}

char* net_utils_ether_ntoa(const uint8_t* ether_addr) {
    static char addr_buf[18];

    addr_buf[17] = 0;

    const int ret = snprintf(
        (char *)addr_buf,
        sizeof(addr_buf),
        "%02x:%02x:%02x:%02x:%02x:%02x",
        ether_addr[0],
        ether_addr[1],
        ether_addr[2],
        ether_addr[3],
        ether_addr[4],
        ether_addr[5]
    );

    if (ret < 0) {
        log_perror("snprintf() failed");
        return nullptr;
    }

    return addr_buf;
}
