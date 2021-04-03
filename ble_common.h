//
// Created by unoco on 4/9/2021.
//

#ifndef OSDP_LIB_BLE_COMMON_H
#define OSDP_LIB_BLE_COMMON_H

#define CONFIG_BT_ENABLED

class IIdentificationObserver {
public:
    virtual void onIdentify(uint8_t *bytes, size_t len) = 0;
};


#endif //OSDP_LIB_BLE_COMMON_H
