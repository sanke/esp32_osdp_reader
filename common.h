//
// Created by unoco on 4/2/2021.
//

#ifndef OSDP_LIB_COMMON_H
#define OSDP_LIB_COMMON_H

#define DOKODA_MANUFACTURER_CODE 0xD0C0DA
#define OSDP_SOM 0x53
#define OSDP_HEADER_LEN 0x05
#define OSDP_PACKET_BUF_SIZE 128
#define OSDP_CAPABILITY_COUNT 12


struct osdp_packet_header {
    uint8_t som;
    uint8_t pd_address;
    uint8_t len_lsb;
    uint8_t len_msb;
    uint8_t control;
};

struct __attribute__((packed)) osdp_pd_capability {
    uint8_t code;
    uint8_t comp_level;
    uint8_t number_of;
};

struct osdp_secure_channel {
    uint8_t scbk[16];
    uint8_t s_enc[16];
    uint8_t s_mac1[16];
    uint8_t s_mac2[16];
    uint8_t r_mac[16];
    uint8_t c_mac[16];
    uint8_t cp_random[8];
    uint8_t pd_random[8];
    uint8_t pd_client_uid[8];
    uint8_t cp_cryptogram[16];
    uint8_t pd_cryptogram[16];
};

struct osdp_pd {
    uint8_t address;
    uint8_t reply_id;
    uint8_t flags;
    osdp_pd_capability cap;
    osdp_secure_channel sc;
};

struct __attribute__((packed)) osdp_pd_id {
    uint8_t vendor_code1;
    uint8_t vendor_code2;
    uint8_t vendor_code3;
    uint8_t model;
    uint8_t version;
    uint32_t serial_number;
    uint8_t fw_major;
    uint8_t fw_minor;
    uint8_t fw_build;
};

enum osdp_led_color_e {
    OSDP_LED_COLOR_NONE,
    OSDP_LED_COLOR_RED,
    OSDP_LED_COLOR_GREEN,
    OSDP_LED_COLOR_AMBER,
    OSDP_LED_COLOR_BLUE,
    OSDP_LED_COLOR_SENTINEL
};

struct __attribute__((packed)) osdp_cmd_led_params {
    uint8_t control_code;
    uint8_t on_count;
    uint8_t off_count;
    uint8_t on_color;
    uint8_t off_color;
    uint16_t timer_count;
};

struct osdp_cmd_led {
    uint8_t reader;
    uint8_t led_number;
    struct osdp_cmd_led_params temporary;
    struct osdp_cmd_led_params permanent; // permanent doesn't have time setting
};

struct osdp_cmd_buzzer {
    uint8_t reader;
    uint8_t control_code;
    uint8_t on_count;
    uint8_t off_count;
    uint8_t rep_count;
};

enum pd_cap_code {
    OSDP_PD_CAP_UNUSED,
    OSDP_PD_CAP_CONTACT_STATUS_MONITORING,
    OSDP_PD_CAP_OUTPUT_CONTROL,
    OSDP_PD_CAP_CARD_DATA_FORMAT,
    OSDP_PD_CAP_READER_LED_CONTROL,
    OSDP_PD_CAP_READER_AUDIBLE_OUTPUT,
    OSDP_PD_CAP_READER_TEXT_OUTPUT,
    OSDP_PD_CAP_TIME_KEEPING,
    OSDP_PD_CAP_CHECK_CHARACTER_SUPPORT,
    OSDP_PD_CAP_COMMUNICATION_SECURITY,
    OSDP_PD_CAP_RECEIVE_BUFFERSIZE,
    OSDP_PD_CAP_LARGEST_COMBINED_MESSAGE_SIZE,
    OSDP_PD_CAP_SMART_CARD_SUPPORT,
    OSDP_PD_CAP_READERS,
    OSDP_PD_CAP_BIOMETRICS,
    OSDP_PD_CAP_SENTINEL
};

/**
 * @brief OSDP reserved commands
 */
#define CMD_POLL                0x60
#define CMD_ID                  0x61
#define CMD_CAP                 0x62
#define CMD_DIAG                0x63
#define CMD_LSTAT               0x64
#define CMD_ISTAT               0x65
#define CMD_OSTAT               0x66
#define CMD_RSTAT               0x67
#define CMD_OUT                 0x68
#define CMD_LED                 0x69
#define CMD_BUZ                 0x6A
#define CMD_TEXT                0x6B
#define CMD_RMODE               0x6C
#define CMD_TDSET               0x6D
#define CMD_COMSET              0x6E
#define CMD_DATA                0x6F
#define CMD_XMIT                0x70
#define CMD_PROMPT              0x71
#define CMD_SPE                 0x72
#define CMD_BIOREAD             0x73
#define CMD_BIOMATCH            0x74
#define CMD_KEYSET              0x75
#define CMD_CHLNG               0x76
#define CMD_SCRYPT              0x77
#define CMD_CONT                0x79
#define CMD_ABORT               0x7A
#define CMD_MAXREPLY            0x7B
#define CMD_MFG                 0x80
#define CMD_SCDONE              0xA0
#define CMD_XWR                 0xA1

/**
 * @brief OSDP reserved responses
 */
#define REPLY_ACK               0x40
#define REPLY_NAK               0x41
#define REPLY_PDID              0x45
#define REPLY_PDCAP             0x46
#define REPLY_LSTATR            0x48
#define REPLY_ISTATR            0x49
#define REPLY_OSTATR            0x4A
#define REPLY_RSTATR            0x4B
#define REPLY_RAW               0x50
#define REPLY_FMT               0x51
#define REPLY_PRES              0x52
#define REPLY_KEYPPAD           0x53
#define REPLY_COM               0x54
#define REPLY_SCREP             0x55
#define REPLY_SPER              0x56
#define REPLY_BIOREADR          0x57
#define REPLY_BIOMATCHR         0x58
#define REPLY_CCRYPT            0x76
#define REPLY_RMAC_I            0x78
#define REPLY_MFGREP            0x90
#define REPLY_BUSY              0x79
#define REPLY_XRD               0xB1


#endif //OSDP_LIB_COMMON_H
