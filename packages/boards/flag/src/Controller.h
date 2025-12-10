//
// Created by kali on 9/2/25.
//

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <hardware/Flags.h>
#include <hardware/Battery.h>
#include <communication/FlagNow.h>
#include <utils/CountingTicker.h>
#include "ArduinoJson.h"
#include <cmath>
#include <vector>

class Controller {
private:
    std::vector<Flags*> flags;
    FlagNow* now;
    Battery* battery;
    bool forceShowBattery = true;
    bool dataReceived = false;
    CountingTicker* batteryTicker;
    CountingTicker* identifyTicker = nullptr;
    String color1, color2, color3, color4;
    void tickBattery(int count);
    void tickIdentify(int count);
    void showBatteryTimerUpdate(double percentage);
    void onReceiveData(String data);
    void onReceiveColor(String c1, String c2, String c3, String c4);
    void onReceiveIdentify();
    void updateColor();
    void showColorOnFlags(String c1, String c2, String c3, String c4);
    void initFlags();
    double getBatteryPercentage();
public:
    Controller();
    bool init();
    Controller* addFlag(Flags* flag);
    Controller* setBattery(Battery* battery);
};



#endif //CONTROLLER_H
