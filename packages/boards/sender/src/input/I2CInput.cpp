//
// Created by kali on 9/1/25.
//

#include "I2CInput.h"

I2CInput::I2CInput(uint8_t i2cAdress, int i2cSdaPin, int i2cSclPin) {
    this->i2c = new I2C(i2cAdress, i2cSdaPin, i2cSclPin, std::bind(&I2CInput::onReceive, this, std::placeholders::_1));
}
void I2CInput::init() {
    this->i2c->init();
    Serial.println("Initialized Input");
}
void I2CInput::onReceive(String data) {
    JsonDocument doc;
    deserializeJson(doc, data);
	JsonArray array = doc.as<JsonArray>();

    State state = createState(array[0].as<int>(), array[1].as<int>() == 1);

    this->callListeners(state);
}
void I2CInput::onChange(OnInputChange onChange) {
    this->listeners.insert(this->listeners.end(), onChange);
}
void I2CInput::callListeners(State state) {
    if (state == this->lastState) {
        return;
    }
    this->lastState = state;
    for (auto & element : this->listeners) {
        element(state);
    }
}