//
// Created by unoco on 4/3/2021.
//

#include "osdp_packet.h"
#include "osdp_utils.h"

void osdp_packet::write(const uint8_t *data, size_t len) {
    memcpy(ptr, data, len);
    ptr += len;
    current_len += len;
}

osdp_packet::osdp_packet(uint8_t cmd, uint8_t address, uint8_t seq) {
    memset(buffer, 0, OSDP_PACKET_BUF_SIZE);
    header = (osdp_packet_header *) buffer;
    current_len = OSDP_HEADER_LEN;

    if (!has_scb()) {
        buffer[current_len++] = cmd;
    }

    ptr = buffer;
    ptr += current_len;

    header->som = OSDP_SOM;
    header->pd_address = address | 0x80;
    header->control = seq & 0x3;
}

osdp_packet::osdp_packet(uint8_t *data, size_t packet_length) {
    memcpy(buffer, data, packet_length);
    ptr = buffer;
    header = (osdp_packet_header *) buffer;
    current_len = packet_length;
}

uint8_t *osdp_packet::get_data() {
    header->len_lsb = (current_len + 1) & 0xff;
    header->len_msb = (current_len + 1) >> 8;

    uint8_t checksum = osdp_utils::checksum(buffer, current_len);

    buffer[current_len++] = checksum;

    return buffer;
}

const uint8_t *osdp_packet::getBuffer() const {
    return buffer;
}

const uint8_t *osdp_packet::get_payload() {
    size_t data_start = OSDP_HEADER_LEN + (has_scb() ? 3 : 0) + 1;
    return buffer + data_start;
}

size_t osdp_packet::get_payload_len() {
    size_t data_start = OSDP_HEADER_LEN + (has_scb() ? 3 : 0) + 1;
    return get_len() - data_start - 1;
}

osdp_packet::osdp_packet(uint8_t cmd) {
    memset(buffer, 0, OSDP_PACKET_BUF_SIZE);
    header = (osdp_packet_header *) buffer;
    current_len = OSDP_HEADER_LEN;

    if (!has_scb()) {
        buffer[current_len++] = cmd;
    }

    ptr = buffer;
    ptr += current_len;

    header->som = OSDP_SOM;
}



