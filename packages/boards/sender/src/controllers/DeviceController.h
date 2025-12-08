//
// Created by mrohmer on 08.12.25.
//

#ifndef COCKPITXP_BREAKOUT_BOARD_ADDON_DEVICECONTROLLER_H
#define COCKPITXP_BREAKOUT_BOARD_ADDON_DEVICECONTROLLER_H

#include <map>
#include "communication/ble/Ble.h"
#include "communication/Now.h"
#include "ArduinoJson.h"

struct FlagDevice {
    String macAddress;
    long lastPing;
    double batteryPercentage;
};


class DeviceController {
private:
    std::map<String, FlagDevice> flagDevices;
    Ble* ble;
    Now* now;

    void onReceiveData(uint8_t* macAddress, String payload);
    void onReceivePing(uint8_t* mac, JsonObject payload);
    void publishToBle();
    String serialiseDevice(FlagDevice device);
public:
    DeviceController(Now* now, Ble* ble);
    void init();
};


#endif //COCKPITXP_BREAKOUT_BOARD_ADDON_DEVICECONTROLLER_H