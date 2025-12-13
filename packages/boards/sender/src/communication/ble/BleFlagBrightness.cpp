//
// Created by mrohmer on 12.12.25.
//

#include "BleFlagBrightness.h"


BleFlagBrightness::BleFlagBrightness(BLEService* service) {
    this->characteristicBrightness = service->createCharacteristic(
        BLE_FLAGS_BRIGHTNESS_CHARACTERISTICS_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY |
        BLECharacteristic::PROPERTY_INDICATE
    );
    this->characteristicBrightness->addDescriptor(new BLE2902());
    this->characteristicBrightness->setCallbacks(this);
}
void BleFlagBrightness::onWrite(BLECharacteristic* characteristic) {
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
    this->characteristicBrightness->notify();
}
BleFlagBrightness* BleFlagBrightness::setValue(int brightness) {
    String payload = String(brightness);
    if (payload == this->characteristicBrightness->getValue()) {
        return this;
    }
    this->characteristicBrightness->setValue(payload);
    this->characteristicBrightness->notify();
    return this;
}
