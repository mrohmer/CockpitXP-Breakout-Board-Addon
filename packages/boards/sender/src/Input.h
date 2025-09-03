//
// Created by kali on 9/1/25.
//

#ifndef INPUT_H
#define INPUT_H

#include <models/State.h>
#include <communication/I2C.h>
#include "ArduinoJson.h"
#include <vector>


typedef std::function<void(State*)> OnChange;
class Input {
private:
    I2C* i2c;
    State* state;
    std::vector<OnChange> listeners;
    void onReceive(String data);
    void callListeners();
public:
    Input(uint8_t i2cAdress, int i2cSdaPin, int i2cSclPin);
    void init();
    void onChange(OnChange onChange);
};



#endif //INPUT_H
