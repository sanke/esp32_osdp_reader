//
// Created by unoco on 4/3/2021.
//

#ifndef OSDP_LIB_OSDP_UTILS_H
#define OSDP_LIB_OSDP_UTILS_H
#include <Arduino.h>

class osdp_utils {
public:
    static uint8_t checksum(const uint8_t *buffer, size_t len);
};


#endif //OSDP_LIB_OSDP_UTILS_H
