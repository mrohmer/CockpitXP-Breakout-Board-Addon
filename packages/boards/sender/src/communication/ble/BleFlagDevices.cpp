//
// Created by mrohmer on 08.12.25.
//

#include "BleFlagDevices.h"

BleFlagDevices::BleFlagDevices(BLEServer* server) {
    BLEService* service = server->createService(BLEUUID(BLE_FLAGS_DEVICES_SERVICE_UUID), 20);

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

    this->characteristicLightToggle = service->createCharacteristic(
        BLE_FLAG_DEVICES_LIGHT_TOGGLE_CHARACTERISTICS_UUID,
        BLECharacteristic::PROPERTY_WRITE_NR |
        BLECharacteristic::PROPERTY_INDICATE
    );
    this->characteristicLightToggle->addDescriptor(new BLE2902());
    this->characteristicLightToggle->setCallbacks(this);

    service->start();
}
void BleFlagDevices::onWrite(BLECharacteristic* characteristic) {
    if (characteristic->getUUID().toString() == BLE_FLAG_DEVICES_IDENTIFY_CHARACTERISTICS_UUID) {
        this->onControlIdentify(characteristic);
    }
    if (characteristic->getUUID().toString() == BLE_FLAG_DEVICES_LIGHT_TOGGLE_CHARACTERISTICS_UUID) {
        this->onControlLightToggle(characteristic);
    }
}
void BleFlagDevices::onControlIdentify(BLECharacteristic* characteristic) {
    String payload = characteristic->getValue();

    this->callIdentifyListeners(payload);
}
void BleFlagDevices::onControlLightToggle(BLECharacteristic* characteristic) {
    String payload = characteristic->getValue();

    this->callLightToggleListeners(payload);
}
void BleFlagDevices::callIdentifyListeners(String macAddress) {
    for (auto & element : this->identifyListeners) {
        element(macAddress);
    }
}
void BleFlagDevices::callLightToggleListeners(String macAddress) {
    for (auto & element : this->lightToggleListeners) {
        element(macAddress);
    }
}
void BleFlagDevices::onIdentify(OnBleIdentify callback) {
    this->identifyListeners.insert(this->identifyListeners.end(), callback);
}
void BleFlagDevices::onLightToggle(OnBleLightToggle callback) {
    this->lightToggleListeners.insert(this->lightToggleListeners.end(), callback);
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
String BleFlagDevices::getServiceUUID() {
    return BLE_FLAGS_DEVICES_SERVICE_UUID;
}