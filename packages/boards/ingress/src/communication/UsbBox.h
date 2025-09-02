//
// Created by kali on 9/2/25.
//

#ifndef USBBOX_H
#define USBBOX_H

#include <Arduino.h>
#include <FunctionalInterrupt.h>

typedef std::function<void(int)> OnChangeCallback;
typedef std::function<void()> InterruptCallback;

class UsbBox {
private:
    int pinFlags1, pinFlags2, pinSessionRecord;
    OnChangeCallback flagsCallback;
    OnChangeCallback sessionRecordCallback;

    UsbBox* initPin(int pin, InterruptCallback callback);
    void onFlagsInterrup();
    void onSessionRecordInterrup();
public:
    UsbBox(int pinFlags1, int pinFlags2, int pinSessionRecord);
    UsbBox* init();
    UsbBox* onFlagsChange(OnChangeCallback callback);
    UsbBox* onSessionRecordChange(OnChangeCallback callback);
};



#endif //USBBOX_H
