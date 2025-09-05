//
// Created by kali on 9/5/25.
//

#ifndef USBBOXINPUT_H
#define USBBOXINPUT_H

#include "Input.h"
#include <Arduino.h>
#include <FunctionalInterrupt.h>
#include <models/State.h>
#include <vector>

typedef std::function<void()> InterruptCallback;
class UsbBoxInput : public Input {
    State lastState = createState(0, false);
    State currentState = createState(0, false);
    std::vector<OnInputChange> listeners;
    int pinFlags1, pinFlags2, pinSessionRecord;
    UsbBoxInput* initPin(int pin, InterruptCallback callback);
    void callListeners(State state);
    void onFlagsInterrup();
    void onSessionRecordInterrup();
public:
    UsbBoxInput(int pinFlags1, int pinFlags2, int pinSessionRecord);
    void init();
    void loop();
    void onChange(OnInputChange onChange);
};

#endif //USBBOXINPUT_H
