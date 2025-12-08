//
// Created by mrohmer on 08.12.25.
//

#include "BleFlagDevices.h"

BleFlagDevices::BleFlagDevices(BLEService* service) {
    characteristicDeviceState = service->createCharacteristic(
        BLE_FLAG_DEVICES_STATE_CHARACTERISTICS_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_NOTIFY |
        BLECharacteristic::PROPERTY_INDICATE
    );
    this->characteristicDeviceState->addDescriptor(new BLE2902());
    this->characteristicIdentify = service->createCharacteristic(
        BLE_FLAG_DEVICES_IDENTIFY_CHARACTERISTICS_UUID,
        BLECharacteristic::PROPERTY_WRITE_NR |
        BLECharacteristic::PROPERTY_INDICATE
    );
    this->characteristicIdentify->addDescriptor(new BLE2902());
    this->characteristicIdentify->setCallbacks(this);
}
void BleFlagDevices::onWrite(BLECharacteristic* characteristic) {
    if (characteristic->getUUID().toString() == BLE_FLAG_DEVICES_IDENTIFY_CHARACTERISTICS_UUID) {
        this->onControlIdentify(characteristic);
    }
}
void BleFlagDevices::onControlIdentify(BLECharacteristic* characteristic) {
    String payload = characteristic->getValue();

    this->callListeners(payload);
}
void BleFlagDevices::callListeners(String macAddress) {
    for (auto & element : this->listeners) {
        element(macAddress);
    }
}
void BleFlagDevices::notify() {
    this->characteristicDeviceState->notify();
}
void BleFlagDevices::setValue(String payload) {
    if (payload == this->characteristicDeviceState->getValue()) {
        return;
    }
    this->characteristicDeviceState->setValue(payload);
    this->characteristicDeviceState->notify();
}