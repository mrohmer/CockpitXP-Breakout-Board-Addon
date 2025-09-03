#ifndef I2C_H
#define I2C_H

#include <Wire.h>
#include <Arduino.h>


class I2C {
private:
    bool needsSend = false;
    int sdaPin;
    int sclPin;
public:
#ifdef ESP32
    I2C(int sdaPin, int sclPin);
    void send(String data);
#else
    void send(char* data);
#endif
    void init();
    void registerSend();
};



#endif //I2C_H
