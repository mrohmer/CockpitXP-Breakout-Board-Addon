//
// Created by kali on 8/31/25.
//

#ifndef I2C_H
#define I2C_H

#include "Wire.h"
#include <Arduino.h>
#include <functional>

typedef std::function<void(String)> Callback;

class I2C {
private:
    static I2C* instance;
    uint8_t address;
    uint8_t sdaPin;
    uint8_t sclPin;
    Callback onReceiveCallback;
    void onReceive(int length);

    static void staticOnReceive(int bytes) {
        Serial.println("I2C::staticOnReceive");
        if (instance) {
            instance->onReceive(bytes);
        }
    }
public:
    I2C(uint8_t address, uint8_t sdaPin, uint8_t sclPin, Callback onReceiveCallback);
    void init();
};

#endif //I2C_H
