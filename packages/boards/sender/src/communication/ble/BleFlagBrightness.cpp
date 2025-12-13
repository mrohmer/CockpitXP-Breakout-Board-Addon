//
// Created by mrohmer on 12.12.25.
//

#include "BleFlagBrightness.h"


BleFlagBrightness::BleFlagBrightness(BLEServer* server) {
    BLEService* service = server->createService(BLEUUID(BLE_FLAGS_BRIGHTNESS_SERVICE_UUID), 20);

    this->characteristicBrightness = service->createCharacteristic(
        BLE_FLAGS_BRIGHTNESS_CHARACTERISTICS_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY |
        BLECharacteristic::PROPERTY_INDICATE
    );
    this->characteristicBrightness->addDescriptor(new BLE2902());
    this->characteristicBrightness->setCallbacks(this);

    service->start();
}
void BleFlagBrightness::onWrite(BLECharacteristic* characteristic) {
    Serial.printf("onWrite %s '%s'\n", characteristic->getUUID().toString().c_str(), characteristic->getValue().c_str());
    if (characteristic->getUUID().toString() == BLE_FLAGS_BRIGHTNESS_CHARACTERISTICS_UUID) {
        this->onBrightnessChange(characteristic);
    }
}
void BleFlagBrightness::onBrightnessChange(BLECharacteristic* characteristic) {
    String payload = characteristic->getValue();

    this->callListeners(atoi(payload.c_str()));
}
BleFlagBrightness* BleFlagBrightness::onChange(OnBleFlagBrightnessChange callback) {
    this->listeners.insert(this->listeners.end(), callback);
    return this;
}
void BleFlagBrightness::callListeners(int brightness) {
    for (auto & element : this->listeners) {
        element(brightness);
    }
}
void BleFlagBrightness::notify() {
    Serial.printf("notify: %s\n", this->characteristicBrightness->getValue());
    this->characteristicBrightness->notify();
}
BleFlagBrightness* BleFlagBrightness::setValue(int brightness) {
    String payload = String(brightness);
    Serial.printf("setValue: %s\n", payload);
    if (payload == this->characteristicBrightness->getValue()) {
        return this;
    }
    this->characteristicBrightness->setValue(payload);
    this->characteristicBrightness->notify();
    return this;
}
String BleFlagBrightness::getServiceUUID() {
    return BLE_FLAGS_BRIGHTNESS_SERVICE_UUID;
}
