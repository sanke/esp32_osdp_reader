//
// Created by unoco on 4/3/2021.
//

#include "osdp_utils.h"

uint8_t osdp_utils::checksum(const uint8_t *buffer, size_t len) {
    uint32_t agr = 0;

    for (int i = 0; i < len; i++) {
        agr += buffer[i];
    }

    return (uint8_t) 0x100 - (agr & 0xff);
}
