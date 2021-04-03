#include "osdp_lib.h"
#include "osdp_utils.h"

osdp_lib::osdp_lib(Stream &stream, Stream &debug, int transfer_pin, int r_pin, int g_pin, int b_pin, int buz_pin)
        : debug(debug), stream(stream), tx_pin(transfer_pin), led_driver(r_pin, g_pin, b_pin), pd(), buffer(),
          outgoing_packets(), buz_driver(buz_pin) {
    pd.address = 0x01;
    buffer_cursor = 0;
    can_write = true;
    has_init = false;
    pinMode(tx_pin, OUTPUT);
}

void osdp_lib::loop() {
    led_driver.loop();
    buz_driver.loop();

    size_t available = stream.available();
    available = available > OSDP_PACKET_BUF_SIZE ? OSDP_PACKET_BUF_SIZE : available;

    if (available > 0) {
        int byte = this->stream.read();

        while (byte >= 0) {
            buffer[buffer_cursor++] = byte;
            byte = this->stream.read();
        }

        if (buffer_cursor >= OSDP_PACKET_BUF_SIZE) {
            buffer_cursor = 0;
            println("Buffer size limit reached ", OSDP_PACKET_BUF_SIZE);
            return;
        } else if (buffer_cursor % 16 == 0) {
            println("Raw buffer: ", buffer, buffer_cursor);
        }

        for (int i = 0; i < buffer_cursor; i++) {
            if (buffer[i] == OSDP_SOM && buffer_cursor - i >= OSDP_HEADER_LEN) {
                uint8_t *ptr = buffer + i;
                auto *header = (osdp_packet_header *) (ptr);
                size_t packet_len = header->len_msb << 8 | header->len_lsb;

                if (buffer_cursor - i < packet_len) {
                    return;
                }

                if (header->pd_address != pd.address) {
                    buffer_cursor = 0;
                    return;
                }

                osdp_packet packet(ptr, packet_len);

                if (packet.get_cmd() != CMD_POLL) {
                    println("Packet: ", ptr, packet_len);
                    println("Incoming command id: ", packet.get_cmd(), 16);

                    if (!packet.has_16bit_crc()) {
                        println("Checksum: ", osdp_utils::checksum(ptr, packet_len - 1), 16);
                    }
                }

                // packet reminder
                if (buffer_cursor - packet_len > 0) {
                    memcpy(buffer, buffer + packet_len, buffer_cursor - packet_len);
                }

                if (has_init || (packet.get_cmd() == CMD_ID)) {
                    process_packet(packet.get_cmd(), packet);
                }

                buffer_cursor = 0;
                return;
            }
        }
    }
}

void osdp_lib::println(const char *str, int value, int base) {
#ifndef NO_OSDP_DEBUG
    debug.print(str);
    if (base == 16) {
        debug.print("0x");
    }

    debug.println(value, base);
#endif
}

void osdp_lib::println(const char *str, const uint8_t *arr, int len) {
#ifndef NO_OSDP_DEBUG
    debug.println(str);
    for (int i = 0; i < len; i++) {
        debug.print(" 0x");
        debug.print(arr[i], 16);
    }
    debug.println();
#endif
}

void osdp_lib::process_packet(uint8_t command, osdp_packet packet) {
    switch (command) {
        case CMD_CAP:
            pd_cap_report(packet);
            break;
        case CMD_ID:
            pd_id_report(packet);
            break;
        case CMD_LED:
            pd_led(packet);
            break;
        case CMD_LSTAT:
            pd_local_status(packet);
            break;
        case CMD_RSTAT:
            pd_reader_status(packet);
            break;
        case CMD_BUZ:
            pd_buz(packet);
            break;
        case CMD_POLL:
            pd_poll(packet);
            break;
    }
}

void osdp_lib::write(uint8_t *buffer, int len, bool log) {
    if (!can_write) {
        return;
    }

    if (log) {
        println("Writing:", buffer, len);
    }

    digitalWrite(tx_pin, HIGH);
    stream.write(0xff);
    stream.write(buffer, len);
    stream.flush();
    digitalWrite(tx_pin, LOW);
}

void osdp_lib::enableWrite() {
    can_write = true;
}

void osdp_lib::disableWrite() {
    can_write = false;
}

void osdp_lib::pd_id_report(osdp_packet in_packet) {
    osdp_packet packet(REPLY_PDID, pd.address, in_packet.get_seq());
    osdp_pd_id id{};

    has_init = true;

    id.vendor_code1 = DOKODA_MANUFACTURER_CODE >> 0 & 0xff;
    id.vendor_code2 = DOKODA_MANUFACTURER_CODE >> 8 & 0xff;
    id.vendor_code3 = DOKODA_MANUFACTURER_CODE >> 16 & 0xff;
    id.model = 0x00;
    id.version = 0x01;
    id.serial_number = 0x00000001;
    id.fw_major = 0x01;
    id.fw_minor = 0x01;
    id.fw_build = 0x00;

    packet.write((const uint8_t *) &id, sizeof(osdp_pd_id));

    write(packet.get_data(), packet.get_len());
}

void osdp_lib::pd_cap_report(osdp_packet in_packet) {
    osdp_packet packet(REPLY_PDCAP, pd.address, in_packet.get_seq());
    osdp_pd_capability caps[OSDP_CAPABILITY_COUNT];

    caps[0].code = OSDP_PD_CAP_CONTACT_STATUS_MONITORING;
    caps[0].comp_level = 0x04;
    caps[0].number_of = 0x01;

    caps[1].code = OSDP_PD_CAP_OUTPUT_CONTROL;
    caps[1].comp_level = 0x04;
    caps[1].number_of = 0x01;

    caps[2].code = OSDP_PD_CAP_CARD_DATA_FORMAT;
    caps[2].comp_level = 0x01;
    caps[2].number_of = 0x00;

    caps[3].code = OSDP_PD_CAP_READER_LED_CONTROL;
    caps[3].comp_level = 0x04;
    caps[3].number_of = 0x01;

    caps[4].code = OSDP_PD_CAP_READER_AUDIBLE_OUTPUT;
    caps[4].comp_level = 0x02;
    caps[4].number_of = 0x01;

    caps[5].code = OSDP_PD_CAP_READER_TEXT_OUTPUT;
    caps[5].comp_level = 0x00;
    caps[5].number_of = 0x00;

    caps[6].code = OSDP_PD_CAP_TIME_KEEPING;
    caps[6].comp_level = 0x00;
    caps[6].number_of = 0x00;

    caps[7].code = OSDP_PD_CAP_CHECK_CHARACTER_SUPPORT;
    caps[7].comp_level = 0x01;
    caps[7].number_of = 0x00;

    caps[8].code = OSDP_PD_CAP_COMMUNICATION_SECURITY;
    caps[8].comp_level = 0x01;
    caps[8].number_of = 0x01;

    caps[9].code = OSDP_PD_CAP_RECEIVE_BUFFERSIZE;
    caps[9].comp_level = 0x02;
    caps[9].number_of = 0x03;

    caps[10].code = OSDP_PD_CAP_LARGEST_COMBINED_MESSAGE_SIZE;
    caps[10].comp_level = 0x02;
    caps[10].number_of = 0x03;

    caps[11].code = OSDP_PD_CAP_SMART_CARD_SUPPORT;
    caps[11].comp_level = 0x00;
    caps[11].number_of = 0x00;

//    caps[12].code = OSDP_PD_CAP_READERS;
//    caps[12].comp_level = 0x01;
//    caps[12].number_of = 0x01;

//    caps[13].code = OSDP_PD_CAP_BIOMETRICS;
//    caps[13].comp_level = 0x00;
//    caps[13].number_of = 0x00;

    packet.write((uint8_t *) caps, sizeof(osdp_pd_capability) * OSDP_CAPABILITY_COUNT);

    write(packet.get_data(), packet.get_len());
}

void osdp_lib::pd_ack(osdp_packet in_packet) {
    osdp_packet packet(REPLY_ACK, pd.address, in_packet.get_seq());
    write(packet.get_data(), packet.get_len(), false);
}

void osdp_lib::pd_local_status(osdp_packet in_packet) {
    osdp_packet packet(REPLY_LSTATR, pd.address, in_packet.get_seq());
    uint8_t status[2];

    status[0] = 0x00; // tamper
    status[1] = 0x00; // power

    packet.write(status, 2);

    write(packet.get_data(), packet.get_len());
}

void osdp_lib::pd_reader_status(osdp_packet in_packet) {
    osdp_packet packet(REPLY_RSTATR, pd.address, in_packet.get_seq());
    uint8_t status[1];

    status[0] = 0x00; // tamper

    packet.write(status, 1);

    write(packet.get_data(), packet.get_len());
}

void osdp_lib::pd_poll(osdp_packet in_packet) {

    if (!outgoing_packets.empty()) {
        osdp_packet &packet = outgoing_packets.front();
        packet.set_seq(in_packet.get_seq());
        packet.set_address(pd.address);
        write(packet.get_data(), packet.get_len());
        outgoing_packets.erase(outgoing_packets.begin());
    } else {
        pd_ack(in_packet);
    }
}

void osdp_lib::pd_led(osdp_packet in_packet) {
    const uint8_t *payload = in_packet.get_payload();
    auto *led = (osdp_cmd_led *) payload;

    if (led->permanent.control_code) {
        led_driver.setOffColor(led->permanent.off_color);
        led_driver.setOnColor(led->permanent.on_color);
        led_driver.setOffCount(led->permanent.off_count);
        led_driver.setOnCount(led->permanent.on_count);
    }

    // TODO: handle temp

    pd_ack(in_packet);
}

void osdp_lib::write_packet(osdp_packet packet) {
    outgoing_packets.push_back(packet);
}

void osdp_lib::write_packet(uint8_t cmd, uint8_t *data, size_t len) {
    osdp_packet packet(cmd);
    packet.write(data, len);
    write_packet(packet);
}

void osdp_lib::pd_buz(osdp_packet in_packet) {
    const uint8_t *payload = in_packet.get_payload();
    auto *buz = (osdp_cmd_buzzer *) payload;

    buz_driver.setOffCount(buz->off_count);
    buz_driver.setOnCount(buz->on_count);
    buz_driver.setRepeatCount(buz->rep_count);

    pd_ack(in_packet);
}

