//
// Created by kali on 8/31/25.
//

#include "I2C.h"

I2C* I2C::instance = nullptr;

I2C::I2C(int address, uint8_t sdaPin, uint8_t sclPin, Callback onReceiveCallback) {
    instance = this;
    this->sdaPin = sdaPin;
    this->sclPin = sclPin;
    this->address = address;
    this->onReceiveCallback = onReceiveCallback;
}
void I2C::init() {
    Wire.begin(this->sdaPin, this->sclPin, this->address);
    Wire.onReceive(staticOnReceive);
    Serial.println("Initialized I2C");
}
void I2C::onReceive(int length) {
    Serial.println("I2C::onReceive");
    String data = String(length);
    while (Wire.available()) {
        data += (char)Wire.read();
    }

    this->onReceiveCallback(data);
}
