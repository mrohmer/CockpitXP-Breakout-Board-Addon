//
// Created by mrohmer on 29.11.25.
//

#include "BleControlFlags.h"

BleControlFlags::BleControlFlags(BLEService* service) {
    this->characteristic = service->createCharacteristic(
        BLE_CONTROL_STATE_CHARACTERISTICS_UUID,
        BLECharacteristic::PROPERTY_WRITE_NR |
        BLECharacteristic::PROPERTY_INDICATE
    );
    this->characteristic->addDescriptor(new BLE2902());
    this->characteristic->setCallbacks(this);
}
void BleControlFlags::onWrite(BLECharacteristic* characteristic) {
    String payload = characteristic->getValue();

    if (payload == F("RED")) {
        this->callListeners(true, createState(0, false));
    } else if (payload == F("GREEN")) {
        this->callListeners(true, createState(1, false));
    } else if (payload == F("CHAOS")) {
        this->callListeners(true, createState(2, false));
    } else if (payload == F("FINISH")) {
        this->callListeners(true, createState(3, false));
    } else {
        this->callListeners(false, createState(0, false));
    }
}
void BleControlFlags::onChange(OnBleControlInputChange onChange) {
    this->listeners.insert(this->listeners.end(), onChange);
}
void BleControlFlags::callListeners(bool enabled, State state) {
    for (auto & element : this->listeners) {
        element(enabled, state);
    }
}