//
// Created by unoco on 4/13/2021.
//

#ifndef OSDP_LIB_OSDP_READER_H
#define OSDP_LIB_OSDP_READER_H

#include "osdp_lib.h"
#include "ble_ident_service.h"

class osdp_reader : public IIdentificationObserver {
public:
    osdp_reader(Stream &serial, Stream &rs485_serial, uint8_t tx_mode_pin);

    osdp_reader(Stream &serial, Stream &rs485_serial, uint8_t tx_mode_pin, int red_led_pin, int green_led_pin,
                int blue_led_pin);

    osdp_reader(Stream &serial, Stream &rs485_serial, uint8_t tx_mode_pin, int red_led_pin, int green_led_pin,
                int blue_led_pin, int buz_pin);

    void onIdentify(uint8_t *bytes, size_t card_len) override;

    void loop();

    void init();

private:
    osdp_lib _osdp_lib;
    ble_ident_service _ble_ident;
};


#endif //OSDP_LIB_OSDP_READER_H
