//
// Created by kali on 9/1/25.
//

#ifndef INPUT_H
#define INPUT_H

#include <State.h>
#include <I2C.h>
#include "ArduinoJson.h"

class Input {
private:
    I2C* i2c;
    State* state;
    void onReceive(String data);
public:
    Input(int i2cAdress, int i2cSdaPin, int i2cSclPin, State* state);
    void init();
};



#endif //INPUT_H
