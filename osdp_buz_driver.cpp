//
// Created by unoco on 4/15/2021.
//

#include "osdp_buz_driver.h"

osdp_buz_driver::osdp_buz_driver(uint8_t pin) : pin(pin) {
    if (pin > 0) {
        pinMode(pin, OUTPUT);
    }
}

void osdp_buz_driver::loop() {
    if(pin < 0) {
        return;
    }

    uint32_t now = millis();

    if (rep_count > 0) {
        if (now - last_on < (uint32_t) on_count * 100) {
            digitalWrite(pin, HIGH);
        } else if (now - last_off < (uint32_t) off_count * 100) {
            digitalWrite(pin, LOW);
        } else {
            last_off = now;
            last_on = now;

            rep_count--;
        }
    } else {
        digitalWrite(pin, LOW);
    }
}

void osdp_buz_driver::setOnCount(uint8_t onCount) {
    on_count = onCount;
}

void osdp_buz_driver::setOffCount(uint8_t offCount) {
    off_count = offCount;
}

void osdp_buz_driver::setRepeatCount(uint8_t reps) {
    rep_count = reps;
}
