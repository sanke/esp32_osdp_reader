//
// Created by unoco on 4/8/2021.
//

#include "ble_ident_service.h"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <Arduino.h>

ble_ident_service::ble_ident_service(Stream &debug)
        : _debug(debug), has_connected(false), last_write(0), conn_id(0), _observers(std::vector<IIdentificationObserver*>()) {
}

void ble_ident_service::start() {
    _debug.println("Initializing BLE sub-system");

    BLEDevice::init("dkd.reader");
    BLEServer *pServer = BLEDevice::createServer();
    BLEService *pService = pServer->createService(SERVICE_UID);

    pServer->setCallbacks(this);

    BLECharacteristic *pCharacteristic = pService->createCharacteristic(
            CHARACTERISTIC_UID,
            BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_WRITE_NR
    );

    pCharacteristic->setCallbacks(this);

    pService->start();
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);

//    BLEAdvertisementData data;
//
//    data.addData("Test");
    //pAdvertising->setAdvertisementData(data);
    BLEDevice::startAdvertising();


}

void ble_ident_service::subscribe(IIdentificationObserver *observer) {
    _observers.push_back(observer);
}


void ble_ident_service::onNotify(BLECharacteristic *pCharacteristic) {
    BLECharacteristicCallbacks::onNotify(pCharacteristic);
    _debug.println(pCharacteristic->getValue().c_str());
}

void ble_ident_service::onWrite(BLECharacteristic *pCharacteristic) {

    auto now = millis();
    if (now - last_write < 1000) {
        _debug.println("Write replay");
        return;
    }

    last_write = now;

    BLECharacteristicCallbacks::onWrite(pCharacteristic);

    _debug.print("onWrite:");

    for (auto &obs : _observers) {
        size_t len = pCharacteristic->getData()[0];

        if(len <= 0) {
            return;
        }

        obs->onIdentify(pCharacteristic->getData() + 1, len);
    }
}

void ble_ident_service::onRead(BLECharacteristic *pCharacteristic) {
    BLECharacteristicCallbacks::onRead(pCharacteristic);

    _debug.println("onRead");
}

void
ble_ident_service::onStatus(BLECharacteristic *pCharacteristic, BLECharacteristicCallbacks::Status s, uint32_t code) {
    BLECharacteristicCallbacks::onStatus(pCharacteristic, s, code);

    _debug.println("Status");
}

void ble_ident_service::onConnect(BLEServer *pServer, esp_ble_gatts_cb_param_t *param) {
    BLEServerCallbacks::onConnect(pServer, param);

    if (has_connected) {
        _debug.print("Client already connected");
        return;
    }

    has_connected = true;
    conn_id = param->conf.conn_id;

    _debug.print("Client connected with id:");
    _debug.println(param->conf.conn_id);
}

void ble_ident_service::onDisconnect(BLEServer *pServer) {
    BLEServerCallbacks::onDisconnect(pServer);
    has_connected = false;
    _debug.println("Client disconnected. Start adver.");
    pServer->startAdvertising();
}