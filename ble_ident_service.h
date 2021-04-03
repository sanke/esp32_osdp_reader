//
// Created by unoco on 4/8/2021.
//

#ifndef OSDP_LIB_BLE_IDENT_SERVICE_H
#define OSDP_LIB_BLE_IDENT_SERVICE_H

#define SERVICE_UID         "3ce420b5-d68d-46dd-82f2-b5d21ea24487"
#define CHARACTERISTIC_UID  "fef63b43-99d4-43c9-8667-28b082353331"
#define CHARACTERISTIC_DATA_LEN 20
#include <Stream.h>
#include <vector>
#include "ble_common.h"
#include <BLEServer.h>

class ble_ident_service : public BLEServerCallbacks, public BLECharacteristicCallbacks {
public:
    ble_ident_service(Stream &debug);

    void subscribe(IIdentificationObserver* observer);
    void start();

    void onConnect(BLEServer *pServer, esp_ble_gatts_cb_param_t *param) override;
    void onDisconnect(BLEServer *pServer) override;
    void onNotify(BLECharacteristic *pCharacteristic) override;
    void onWrite(BLECharacteristic *pCharacteristic) override;
    void onRead(BLECharacteristic *pCharacteristic) override;
    void onStatus(BLECharacteristic *pCharacteristic, Status s, uint32_t code) override;

private:
    Stream &_debug;
    std::vector<IIdentificationObserver*> _observers;

    bool has_connected;
    int conn_id;
    unsigned int last_write;
};


#endif //OSDP_LIB_BLE_IDENT_SERVICE_H
