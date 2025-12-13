//
// Created by mrohmer on 12.12.25.
//

#include "BleFlagLightConfig.h"


BleFlagLightConfig::BleFlagLightConfig(BLEServer* server) {
    BLEService* service = server->createService(BLEUUID(BLE_FLAGS_LIGHT_CONFIG_SERVICE_UUID), 20);

    this->characteristicBrightness = service->createCharacteristic(
        BLE_FLAGS_LIGHT_CONFIG_BRIGHTNESS_CHARACTERISTICS_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY |
        BLECharacteristic::PROPERTY_INDICATE
    );
    this->characteristicBrightness->addDescriptor(new BLE2902());
    this->characteristicBrightness->setCallbacks(this);

    this->characteristicChaosStyle = service->createCharacteristic(
        BLE_FLAGS_LIGHT_CONFIG_CHAOS_STYLE_CHARACTERISTICS_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY |
        BLECharacteristic::PROPERTY_INDICATE
    );
    this->characteristicChaosStyle->addDescriptor(new BLE2902());
    this->characteristicChaosStyle->setCallbacks(this);

    service->start();
}
void BleFlagLightConfig::onWrite(BLECharacteristic* characteristic) {
    Serial.printf("onWrite %s '%s'\n", characteristic->getUUID().toString().c_str(), characteristic->getValue().c_str());
    if (characteristic->getUUID().toString() == BLE_FLAGS_LIGHT_CONFIG_BRIGHTNESS_CHARACTERISTICS_UUID) {
        this->onBrightnessChangeBleEvent(characteristic);
    } else if (characteristic->getUUID().toString() == BLE_FLAGS_LIGHT_CONFIG_CHAOS_STYLE_CHARACTERISTICS_UUID) {
        this->onChaosStyleChangeBleEvent(characteristic);
    }
}
void BleFlagLightConfig::onBrightnessChangeBleEvent(BLECharacteristic* characteristic) {
    String payload = characteristic->getValue();

    this->callBrightnessListeners(atoi(payload.c_str()));
}
void BleFlagLightConfig::onChaosStyleChangeBleEvent(BLECharacteristic* characteristic) {
    String payload = characteristic->getValue();

    this->callChaosStyleListeners(payload);
}
BleFlagLightConfig* BleFlagLightConfig::onBrightnessChange(OnBleFlagLightConfigBrightnessChange callback) {
    this->brightnessListeners.insert(this->brightnessListeners.end(), callback);
    return this;
}
BleFlagLightConfig* BleFlagLightConfig::onChaosStyleChange(OnBleFlagLightConfigChaosStyleChange callback) {
    this->chaosStyleListeners.insert(this->chaosStyleListeners.end(), callback);
    return this;
}
void BleFlagLightConfig::callBrightnessListeners(int brightness) {
    for (auto & element : this->brightnessListeners) {
        element(brightness);
    }
}
void BleFlagLightConfig::callChaosStyleListeners(String style) {
    for (auto & element : this->chaosStyleListeners) {
        element(style);
    }
}
void BleFlagLightConfig::notify() {
    this->characteristicBrightness->notify();
    this->characteristicChaosStyle->notify();
}
BleFlagLightConfig* BleFlagLightConfig::setBrightness(int brightness) {
    String payload = String(brightness);
    if (payload == this->characteristicBrightness->getValue()) {
        return this;
    }
    this->characteristicBrightness->setValue(payload);
    this->characteristicBrightness->notify();
    return this;
}
BleFlagLightConfig* BleFlagLightConfig::setChaosStyle(String payload) {
    if (payload == this->characteristicChaosStyle->getValue()) {
        return this;
    }
    this->characteristicChaosStyle->setValue(payload);
    this->characteristicChaosStyle->notify();
    return this;
}
String BleFlagLightConfig::getServiceUUID() {
    return BLE_FLAGS_LIGHT_CONFIG_SERVICE_UUID;
}
