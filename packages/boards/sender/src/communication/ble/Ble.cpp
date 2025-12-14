//
// Created by mrohmer on 29.11.25.
//

#include "Ble.h"

void Ble::init() {
    BLEDevice::setMTU(512);
    BLEDevice::init(F("CMA Center"));
    this->server = BLEDevice::createServer();
    this->server->setCallbacks(this);

    this->controlFlags = new BleControlFlags(this->server);
    this->flagDevices = new BleFlagDevices(this->server);
    this->lightConfig = new BleFlagLightConfig(this->server);

    BLEAdvertising *advertising = BLEDevice::getAdvertising();
    advertising->addServiceUUID(this->controlFlags->getServiceUUID());
    advertising->addServiceUUID(this->flagDevices->getServiceUUID());
    advertising->addServiceUUID(this->lightConfig->getServiceUUID());
    advertising->setScanResponse(false);
    advertising->setMinPreferred(0x0);
    BLEDevice::startAdvertising();
}
void Ble::onConnect(BLEServer *server) {
    connected = true;
    this->callOnConnectionChangeListeners(true);

    // notify characteristcs
    this->controlFlags->notify();
    this->flagDevices->notify();
    this->lightConfig->notify();
}
void Ble::onDisconnect(BLEServer *server) {
    connected = false;
    this->callOnConnectionChangeListeners(false);

    delay(500);
    BLEDevice::startAdvertising();
}
BleControlFlags* Ble::getControlFlags() {
    return this->controlFlags;
}
BleFlagDevices* Ble::getFlagDevices() {
    return this->flagDevices;
}
BleFlagLightConfig* Ble::getLightConfig() {
    return this->lightConfig;
}
void Ble::onConnectionChange(BleOnConnectionChangeCallback callback) {
    this->onConnectionChangeListeners.insert(this->onConnectionChangeListeners.end(), callback);
}
void Ble::callOnConnectionChangeListeners(bool state) {
    for (auto & element : this->onConnectionChangeListeners) {
        element(state);
    }
}
