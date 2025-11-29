//
// Created by mrohmer on 29.11.25.
//

#include "Ble.h"

void Ble::init() {
    BLEDevice::init(F("CMA Center"));
    this->server = BLEDevice::createServer();
    this->server->setCallbacks(this);

    this->service = this->server->createService(BLEUUID(BLE_SERVICE_UUID), 20);

    this->controlFlags = new BleControlFlags(this->service);

    this->service->start();

    BLEAdvertising *advertising = BLEDevice::getAdvertising();
    advertising->addServiceUUID(BLE_SERVICE_UUID);
    advertising->setScanResponse(false);
    advertising->setMinPreferred(0x0);
    BLEDevice::startAdvertising();
}
void Ble::onConnect(BLEServer *server) {
    connected = true;

    // notify characteristcs
    this->controlFlags->notify();
}
void Ble::onDisconnect(BLEServer *server) {
    connected = false;

    delay(500);
    BLEDevice::startAdvertising();
}
BleControlFlags* Ble::getControlFlags() {
    return this->controlFlags;
}
