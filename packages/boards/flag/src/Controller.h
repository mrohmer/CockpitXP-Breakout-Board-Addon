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
#include <vector>

class Controller {
private:
    std::vector<Flags*> flags;
    Now* now;
    CountingTicker* initTicker;
    void onReceiveData(String data);
    void initAnimationTick(int count);
    void initFlags();
public:
    Controller();
    bool init();
    Controller* addFlag(Flags* flag);
};



#endif //CONTROLLER_H
