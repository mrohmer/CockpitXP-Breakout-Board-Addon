//
// Created by kali on 9/2/25.
//

#include "UsbBox.h"

UsbBox::UsbBox(int pinFlags1, int pinFlags2, int pinSessionRecord) {
    this->pinFlags1 = pinFlags1;
    this->pinFlags2 = pinFlags2;
    this->pinSessionRecord = pinSessionRecord;
}
UsbBox* UsbBox::init() {
    return this
        ->initPin(this->pinFlags1, std::bind(&UsbBox::onFlagsInterrup, this))
        ->initPin(this->pinFlags2, std::bind(&UsbBox::onFlagsInterrup, this))
        ->initPin(this->pinSessionRecord, std::bind(&UsbBox::onSessionRecordInterrup, this));
}
UsbBox* UsbBox::initPin(int pin, InterruptCallback callback) {
    pinMode(pin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pin), callback, CHANGE);
    return this;
}
UsbBox* UsbBox::onFlagsChange(FlagsCallback callback) {
    this->flagsCallback = &callback;
    return this;
}
UsbBox* UsbBox::onSessionRecordChange(SessionRecordCallback callback) {
    this->sessionRecordCallback = &callback;
    return this;
}
void UsbBox::onFlagsInterrup() {
    if (this->flagsCallback == nullptr) {
        return;
    }
    uint8_t value1 = 1 - digitalRead(this->pinFlags1);
    uint8_t value2 = 1 - digitalRead(this->pinFlags2);

    auto callback = *this->flagsCallback;
    callback(value1 + value2 * 2);
}
void UsbBox::onSessionRecordInterrup() {
    if (this->sessionRecordCallback == nullptr) {
        return;
    }
    auto callback = *this->sessionRecordCallback;
    callback((1 - digitalRead(PIN_FLAG_SESSION_RECORD)) == 1);
}