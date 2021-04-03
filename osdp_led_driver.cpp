//
// Created by unoco on 4/5/2021.
//

#include "osdp_led_driver.h"

void osdp_led_driver::loop() {
    if (r_led_pin > 0 || g_led_pin > 0 || b_led_pin > 0) {
        uint32_t now = millis();

        if (now - last_on < (uint32_t) on_count * 100) {
            toggle(on_color, HIGH);
            toggle(off_color, LOW);
            last_off = now;
        } else if (now - last_off < (uint32_t) off_count * 100) {
            toggle(off_color, HIGH);
            toggle(on_color, LOW);
        } else {
            last_off = now;
            last_on = now;
        }
    }
}

osdp_led_driver::osdp_led_driver(int r_led, int g_led, int b_led) : r_led_pin(r_led),
                                                                    g_led_pin(g_led),
                                                                    b_led_pin(b_led),
                                                                    on_count(1),
                                                                    off_count(0),
                                                                    has_temporary(false),
                                                                    timer_count(0),
                                                                    off_color(OSDP_LED_COLOR_GREEN),
                                                                    on_color(OSDP_LED_COLOR_BLUE) {
    has_temporary = false;
    last_on = 0;
    last_off = 0;

    if (r_led_pin > 0) {
        pinMode(r_led_pin, OUTPUT);
    }

    if (g_led_pin > 0) {
        pinMode(g_led_pin, OUTPUT);
    }

    if (b_led_pin > 0) {
        pinMode(b_led_pin, OUTPUT);
    }
}

void osdp_led_driver::setOnCount(uint8_t onCount) {
    on_count = onCount;
}

void osdp_led_driver::setOffCount(uint8_t offCount) {
    off_count = offCount;
}

void osdp_led_driver::setOnColor(uint8_t onColor) {
    on_color = onColor;
}

void osdp_led_driver::setOffColor(uint8_t offColor) {
    off_color = offColor;
}

void osdp_led_driver::setTimerCount(uint16_t timerCount) {
    timer_count = timerCount;
}

void osdp_led_driver::toggle(uint8_t color, uint8_t mode) {
    switch (color) {
        case OSDP_LED_COLOR_RED:
        case OSDP_LED_COLOR_AMBER:
            if (r_led_pin < 0) {
                return;
            }

            digitalWrite(r_led_pin, mode);
            break;
        case OSDP_LED_COLOR_GREEN:
            if (g_led_pin < 0) {
                return;
            }

            digitalWrite(g_led_pin, mode);
            break;
        case OSDP_LED_COLOR_SENTINEL:
        case OSDP_LED_COLOR_BLUE:
            if (b_led_pin < 0) {
                return;
            }
            digitalWrite(b_led_pin, mode);
            break;
        case OSDP_LED_COLOR_NONE:
            toggle_all(LOW);
            break;
        default:
            break;
    }
}

void osdp_led_driver::toggle_all(uint8_t mode) {
    if (r_led_pin > 0) {
        digitalWrite(r_led_pin, mode);
    }

    if (g_led_pin > 0) {
        digitalWrite(g_led_pin, mode);
    }

    if (b_led_pin > 0) {
        digitalWrite(b_led_pin, mode);
    }
}
