#ifndef OSDP_LIB_OSDP_LIB_H
#define OSDP_LIB_OSDP_LIB_H

//#define NO_OSDP_DEBUG

#include <Arduino.h>
#include <vector>
#include "Stream.h"
#include "common.h"
#include "osdp_packet.h"
#include "osdp_led_driver.h"
#include "osdp_buz_driver.h"

class osdp_lib {
public:
    osdp_lib(Stream &stream, Stream &debug, int transfer_pin, int r_pin, int g_pin, int b_pin, int buz_pin);
    void write(uint8_t *buffer, int len, bool log = true);

    void loop();
    void enableWrite();
    void disableWrite();

    void write_packet(osdp_packet packet);
    void write_packet(uint8_t cmd, uint8_t *data, size_t len);
private:
    Stream &stream;
    Stream &debug;
    osdp_pd pd;
    uint8_t tx_pin;
    size_t buffer_cursor;
    uint8_t buffer[OSDP_PACKET_BUF_SIZE];
    osdp_led_driver led_driver;
    osdp_buz_driver buz_driver;

    std::vector<osdp_packet> outgoing_packets;

    bool can_write;
    bool has_init;

    void println(const char *, int value, int base = 10);
    void println(const char *, const uint8_t *, int len);
    void process_packet(uint8_t command, osdp_packet packet);
    void pd_id_report(osdp_packet packet);
    void pd_cap_report(osdp_packet in_packet);
    void pd_ack(osdp_packet packet);
    void pd_local_status(osdp_packet in_packet);
    void pd_reader_status(osdp_packet in_packet);
    void pd_poll(osdp_packet in_packet);

    void pd_led(osdp_packet packet);


    void pd_buz(osdp_packet packet);
};

#endif //OSDP_LIB_OSDP_LIB_H
