//
// Created by kali on 9/1/25.
//

#include "Input.h"

Input::Input(uint8_t i2cAdress, int i2cSdaPin, int i2cSclPin) {
    this->i2c = new I2C(i2cAdress, i2cSdaPin, i2cSclPin, std::bind(&Input::onReceive, this, std::placeholders::_1));
    this->state = new State();
}
void Input::init() {
    this->i2c->init();
    Serial.println("Initialized Input");
}
void Input::onReceive(String data) {
    JsonDocument doc;
    deserializeJson(doc, data);
	JsonArray array = doc.as<JsonArray>();

    bool changedFlags = this->state->setFlags(array[0].as<int>());
    bool changedSessionRecord = this->state->setSessionRecord(array[1].as<int>() == 1);

    if (!changedFlags && !changedSessionRecord) {
        return;
    }
    this->callListeners();
}
void Input::onChange(OnChange onChange) {
    this->listeners.insert(this->listeners.end(), onChange);
}
void Input::callListeners() {
    for (auto & element : this->listeners) {
        element(this->state);
    }
}