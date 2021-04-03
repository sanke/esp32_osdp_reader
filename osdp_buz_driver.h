//
// Created by unoco on 4/15/2021.
//

#ifndef OSDP_LIB_OSDP_BUZ_DRIVER_H
#define OSDP_LIB_OSDP_BUZ_DRIVER_H

#include "Arduino.h"

class osdp_buz_driver  {
public:
    osdp_buz_driver(uint8_t pin);
    void setOnCount(uint8_t onCount);
    void setOffCount(uint8_t offCount);
    void loop();

    void setRepeatCount(uint8_t reps);

private:
    uint8_t pin;
    uint32_t last_on;
    uint32_t last_off;
    uint8_t on_count;
    uint8_t off_count;
    uint8_t rep_count;
};


#endif //OSDP_LIB_OSDP_BUZ_DRIVER_H
