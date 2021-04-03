//
// Created by unoco on 4/13/2021.
//

#include "osdp_reader.h"

#define R_LED_PIN 33
#define G_LED_PIN 25
#define B_LED_PIN 26
#define BUZ_PIN 14

osdp_reader::osdp_reader(Stream &serial, Stream &rs485_serial, uint8_t tx_mode_pin)
        : osdp_reader(serial, rs485_serial, tx_mode_pin, R_LED_PIN, G_LED_PIN, B_LED_PIN, BUZ_PIN) {

}

void osdp_reader::onIdentify(uint8_t *bytes, size_t card_len) {
    size_t len = 0;
    uint8_t buffer[128];
    uint16_t bits = card_len * 8;

    buffer[len++] = 0x0;
    buffer[len++] = 0x1;
    buffer[len++] = bits & 0xff;
    buffer[len++] = bits >> 8 & 0xff;

    len += card_len;
    memcpy(buffer + 4, bytes, card_len);
    _osdp_lib.write_packet(REPLY_RAW, buffer, len);
}

void osdp_reader::loop() {
    _osdp_lib.loop();
}

void osdp_reader::init() {
    _ble_ident.start();
}

osdp_reader::osdp_reader(Stream &serial, Stream &rs485_serial, uint8_t tx_mode_pin, int red_led_pin,
                         int green_led_pin, int blue_led_pin)
        : osdp_reader(serial, rs485_serial, tx_mode_pin, red_led_pin, green_led_pin, blue_led_pin, BUZ_PIN) {

}

osdp_reader::osdp_reader(Stream &serial, Stream &rs485_serial, uint8_t tx_mode_pin, int red_led_pin,
                         int green_led_pin, int blue_led_pin, int buz_pin)
        : _osdp_lib((Stream &) rs485_serial, (Stream &) serial, tx_mode_pin, red_led_pin, green_led_pin,
                    blue_led_pin, buz_pin), _ble_ident((Stream &) Serial) {
    _ble_ident.subscribe(this);
}
