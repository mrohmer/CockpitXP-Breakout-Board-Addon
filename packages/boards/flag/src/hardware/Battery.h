//
// Created by kali on 9/22/25.
//

#ifndef BATTERY_H
#define BATTERY_H

#include "Arduino.h"
#include <SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library.h>


class Battery {
private:
    SFE_MAX1704X lipo;
    long lastRead = -1;
    double lastSoc = -1;
    double readPercentage();
public:
    Battery();
    void init();
    double getPercentage();
};



#endif //BATTERY_H
