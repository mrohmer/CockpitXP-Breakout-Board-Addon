#include "I2C.h"

#ifdef ESP32
I2C::I2C(int sdaPin, int sclPin) {
    this->sdaPin = sdaPin;
    this->sclPin = sclPin;
}
#endif
void I2C::init() {
#ifdef ESP32
    Wire.begin(this->sdaPin, this->sclPin);
#else
    Wire.begin();
#endif
}
void I2C::registerSend() {
    this->needsSend = true;
}
#ifdef ESP32
void I2C::send(String data) {
#else
void I2C::send(char* data) {
#endif
    int nDevices = 0;
    int error, address;
    for(address = 1; address < 127; address++ ) {
        Wire.beginTransmission(address);
#ifdef ESP32
        Wire.write((uint8_t*)data.c_str(), data.length());
#else
        Wire.write(data);
#endif
        error = Wire.endTransmission();
        if (error == 0) {
            nDevices++;
        }
        else if (error==4) {
            Serial.print("Unknow error at address 0x");
            if (address<16) {
                Serial.print("0");
            }
            Serial.println(address,HEX);
        }
    }

    Serial.print("Sent to ");
    Serial.print(nDevices);
    Serial.println(" devices.\n");
}
