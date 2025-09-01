//
// Created by kali on 9/1/25.
//

#include "Input.h"

Input::Input(int i2cAdress, int i2cSdaPin, int i2cSclPin, State* state) {
    this->i2c = new I2C(i2cAdress, i2cSdaPin, i2cSclPin, std::bind(&Input::onReceive, this, std::placeholders::_1));
    this->state = state;
}
void Input::init() {
    this->i2c->init();
}
void Input::onReceive(String data) {
    JsonDocument doc;
    deserializeJson(doc, data);

    this->state->setFlags(doc[0].toInt());
    this->state->setSessionRecord(doc[1].toInt() == 1);
}