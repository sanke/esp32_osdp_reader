//
// Created by unoco on 4/5/2021.
//

#ifndef OSDP_LIB_OSDP_LED_DRIVER_H
#define OSDP_LIB_OSDP_LED_DRIVER_H

#include <Arduino.h>
#include "common.h"

class osdp_led_driver {
public:
    osdp_led_driver(int r_led, int g_led, int b_led);

    void loop();
    void setOnCount(uint8_t onCount);
    void setOffCount(uint8_t offCount);
    void setOnColor(uint8_t onColor);
    void setOffColor(uint8_t offColor);
    void setTimerCount(uint16_t timerCount);
private:
    void toggle(uint8_t color, uint8_t);


    uint8_t on_count;
    uint8_t off_count;
    uint8_t on_color;
    uint8_t off_color;
    uint16_t timer_count;

    uint32_t last_on;
    uint32_t last_off;

    int r_led_pin;
    int g_led_pin;
    int b_led_pin;

    bool has_temporary;

    void toggle_all(uint8_t mode);
};


#endif //OSDP_LIB_OSDP_LED_DRIVER_H
