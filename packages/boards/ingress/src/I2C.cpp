#include "I2C.h"

void I2C::init() {
    Wire.begin();
}
void I2C::registerSend() {
    this->needsSend = true;
}
void I2C::send(char* data) {
    int nDevices = 0;
    byte error, address;
    for(address = 1; address < 127; address++ ) {
        Wire.beginTransmission(address);
        Wire.write(data);
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
