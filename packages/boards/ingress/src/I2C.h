#ifndef I2C_H
#define I2C_H

#include <Wire.h>


class I2C {
private:
    bool needsSend = false;
public:
    void init();
    void registerSend();
    void send(char* data);
};



#endif //I2C_H
