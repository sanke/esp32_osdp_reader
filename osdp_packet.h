//
// Created by unoco on 4/3/2021.
//

#ifndef OSDP_LIB_OSDP_PACKET_H
#define OSDP_LIB_OSDP_PACKET_H

#include <Arduino.h>
#include "common.h"

class osdp_packet {
public:
    osdp_packet(const osdp_packet &packet) {
        memcpy(buffer, packet.buffer, OSDP_PACKET_BUF_SIZE);
        header = (osdp_packet_header *) buffer;
        current_len = packet.current_len;
        ptr = buffer + current_len;
    }

    osdp_packet(uint8_t cmd, uint8_t address, uint8_t seq);

    osdp_packet(uint8_t cmd);

    osdp_packet(uint8_t *data, size_t packet_length);

    void write(const uint8_t *data, size_t len);

    uint8_t *get_data();

    size_t get_len() const {
        return current_len;
    }

    void set_seq(uint8_t seq) {
        header->control = seq & 0x3;
    }

    void set_address(uint8_t address) {
        header->pd_address = address | 0x80;
    }

    bool has_16bit_crc() {
        return (header->control & 0x4) != 0;
    }

    bool has_scb() {
        return (header->control & 0x8) != 0;
    }

    uint8_t get_seq() {
        return (header->control & 0x3);
    }

    uint8_t get_address() {
        return (header->pd_address);
    }

    uint8_t get_cmd() {
        if (!has_scb()) {
            return buffer[OSDP_HEADER_LEN];
        } else {
            return -1;
        }
    }

private:
    uint8_t *ptr;
    int current_len;
    osdp_packet_header *header;
    uint8_t buffer[OSDP_PACKET_BUF_SIZE];
public:
    const uint8_t *getBuffer() const;

    const uint8_t *get_payload();

    size_t get_payload_len();
};


#endif //OSDP_LIB_OSDP_PACKET_H
