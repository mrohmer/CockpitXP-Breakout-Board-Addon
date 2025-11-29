//
// Created by mrohmer on 29.11.25.
//

#include "BleControlFlags.h"

BleControlFlags::BleControlFlags(BLEService* service) {
    this->characteristic = service->createCharacteristic(
        BLE_FLAGS_CONTROL_ENABLE_CHARACTERISTICS_UUID,
        BLECharacteristic::PROPERTY_WRITE_NR |
        BLECharacteristic::PROPERTY_INDICATE
    );
    this->characteristic->addDescriptor(new BLE2902());
    this->characteristic->setCallbacks(this);
    this->characteristic = service->createCharacteristic(
        BLE_FLAGS_CONTROL_VALUE_CHARACTERISTICS_UUID,
        BLECharacteristic::PROPERTY_WRITE_NR |
        BLECharacteristic::PROPERTY_INDICATE
    );
    this->characteristic->addDescriptor(new BLE2902());
    this->characteristic->setCallbacks(this);
}
void BleControlFlags::onWrite(BLECharacteristic* characteristic) {
    if (characteristic->getUUID().toString() == BLE_FLAGS_CONTROL_ENABLE_CHARACTERISTICS_UUID) {
        this->onControlEnabledChange(characteristic);
    } else if (characteristic->getUUID().toString() == BLE_FLAGS_CONTROL_VALUE_CHARACTERISTICS_UUID) {
        this->onControlValueChange(characteristic);
    }
}
void BleControlFlags::onControlEnabledChange(BLECharacteristic* characteristic) {
    String payload = characteristic->getValue();

    this->callListeners(payload == "true", this->state);
}
void BleControlFlags::onControlValueChange(BLECharacteristic* characteristic) {
    String payload = characteristic->getValue();

    if (payload == F("RED")) {
        this->callListeners(this->enabled, createState(0, false));
    } else if (payload == F("GREEN")) {
        this->callListeners(this->enabled, createState(1, false));
    } else if (payload == F("CHAOS")) {
        this->callListeners(this->enabled, createState(2, false));
    } else if (payload == F("FINISH")) {
        this->callListeners(this->enabled, createState(3, false));
    }
}
void BleControlFlags::onChange(OnBleControlInputChange onChange) {
    this->listeners.insert(this->listeners.end(), onChange);
}
void BleControlFlags::callListeners(bool enabled, State state) {
    this->enabled = enabled;
    this->state = state;
    for (auto & element : this->listeners) {
        element(enabled, state);
    }
}