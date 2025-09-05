//
// Created by kali on 9/5/25.
//

#include "UsbBoxInput.h"

UsbBoxInput::UsbBoxInput(int pinFlags1, int pinFlags2, int pinSessionRecord) {
    this->pinFlags1 = pinFlags1;
    this->pinFlags2 = pinFlags2;
    this->pinSessionRecord = pinSessionRecord;
}
void UsbBoxInput::init() {
    this
        ->initPin(this->pinFlags1, std::bind(&UsbBoxInput::onFlagsInterrup, this))
        ->initPin(this->pinFlags2, std::bind(&UsbBoxInput::onFlagsInterrup, this))
        ->initPin(this->pinSessionRecord, std::bind(&UsbBoxInput::onSessionRecordInterrup, this));
}
UsbBoxInput* UsbBoxInput::initPin(int pin, InterruptCallback callback) {
    pinMode(pin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pin), callback, CHANGE);
    return this;
}

void UsbBoxInput::onFlagsInterrup() {
    uint8_t value1 = 1 - digitalRead(this->pinFlags1);
    uint8_t value2 = 1 - digitalRead(this->pinFlags2);

    int flags = value1 + value2 * 2;
    this->currentState = createState(flags, this->currentState.isSessionRecord);
}
void UsbBoxInput::onSessionRecordInterrup() {
    bool sessionRecord = (1 - digitalRead(this->pinSessionRecord)) == 1;
    this->currentState = createState(this->currentState.flags, sessionRecord);
}
void UsbBoxInput::onChange(OnInputChange onChange) {
    this->listeners.insert(this->listeners.end(), onChange);
}
void UsbBoxInput::callListeners(State state) {
    if (state == this->lastState) {
        return;
    }
    this->lastState = state;
    for (auto & element : this->listeners) {
        element(state);
    }
}
void UsbBoxInput::loop() {
    this->callListeners(this->currentState);
}