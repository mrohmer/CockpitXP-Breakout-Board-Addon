//
// Created by kali on 9/1/25.
//

#include "Input.h"

Input::Input(uint8_t i2cAdress, int i2cSdaPin, int i2cSclPin) {
    this->i2c = new I2C(i2cAdress, i2cSdaPin, i2cSclPin, std::bind(&Input::onReceive, this, std::placeholders::_1));
}
void Input::init() {
    this->i2c->init();
    Serial.println("Initialized Input");
}
void Input::onReceive(String data) {
    JsonDocument doc;
    deserializeJson(doc, data);
	JsonArray array = doc.as<JsonArray>();

    State state = createState(array[0].as<int>(), array[1].as<int>() == 1);

    if (state == this->lastState) {
        return;
    }
    this->callListeners(state);
}
void Input::onChange(OnChange onChange) {
    this->listeners.insert(this->listeners.end(), onChange);
}
void Input::callListeners(State state) {
    for (auto & element : this->listeners) {
        element(state);
    }
}