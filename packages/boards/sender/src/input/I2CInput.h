//
// Created by kali on 9/1/25.
//

#ifndef I2C_INPUT_H
#define I2C_INPUT_H

#include "Input.h"
#include <models/State.h>
#include <communication/I2C.h>
#include "ArduinoJson.h"
#include <vector>


class I2CInput : public Input {
private:
    I2C* i2c;
    State lastState;
    std::vector<OnInputChange> listeners;
    void onReceive(String data);
    void callListeners(State state);
public:
    I2CInput(uint8_t i2cAdress, int i2cSdaPin, int i2cSclPin);
    void init();
    void loop() {};
    void onChange(OnInputChange onChange);
};



#endif //I2C_INPUT_H
