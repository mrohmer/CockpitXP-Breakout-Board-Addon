//
// Created by kali on 8/31/25.
//

#include "I2C.h"

I2C* I2C::instance = nullptr;

I2C::I2C(uint8_t address, uint8_t sdaPin, uint8_t sclPin, Callback onReceiveCallback) {
    instance = this;
    this->sdaPin = sdaPin;
    this->sclPin = sclPin;
    this->address = address;
    this->onReceiveCallback = onReceiveCallback;
}
void I2C::init() {
    Wire.begin(this->address, this->sdaPin, this->sclPin, 0);
    Wire.onReceive(staticOnReceive);
    Serial.println("Initialized I2C");
}
void I2C::onReceive(int length) {
    Serial.print("I2C::onReceive: ");
    String data = "";
    while (Wire.available()) {
        data += (char)Wire.read();
    }

    Serial.println(data.c_str());
    this->onReceiveCallback(data);
}
