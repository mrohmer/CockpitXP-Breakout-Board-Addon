//
// Created by kali on 9/2/25.
//

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <hardware/Flags.h>
#include <hardware/Battery.h>
#include <communication/Now.h>
#include <utils/CountingTicker.h>
#include "ArduinoJson.h"
#include <cmath>
#include <vector>

class Controller {
private:
    std::vector<Flags*> flags;
    Now* now;
    Battery* battery;
    bool forceShowBattery = true;
    CountingTicker* initTicker;
    void showBatteryTimerUpdate(int count);
    void onReceiveData(String data);
    void initFlags();
    double getBatteryPercentage();
public:
    Controller();
    bool init();
    Controller* addFlag(Flags* flag);
    Controller* setBattery(Battery* battery);
};



#endif //CONTROLLER_H
