//
// Created by kali on 9/2/25.
//

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Flags.h>
#include <communication/Now.h>
#include <utils/CountingTicker.h>
#include "ArduinoJson.h"
#include <cmath>

class Controller {
private:
    Flags* flags;
    Now* now;
    CountingTicker* initTicker;
    void onReceiveData(String data);
    void initAnimationTick(int count);
public:
    Controller(Flags* flags);
    bool init();
};



#endif //CONTROLLER_H
