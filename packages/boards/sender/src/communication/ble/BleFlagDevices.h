//
// Created by mrohmer on 08.12.25.
//

#ifndef COCKPITXP_BREAKOUT_BOARD_ADDON_BLEFLAGDEVICES_H
#define COCKPITXP_BREAKOUT_BOARD_ADDON_BLEFLAGDEVICES_H

#include <BLEServer.h>
#include <BLE2902.h>

#define BLE_FLAG_DEVICES_STATE_CHARACTERISTICS_UUID "b32474c1-42d6-495a-a514-47f48ee72965"
#define BLE_FLAG_DEVICES_IDENTIFY_CHARACTERISTICS_UUID "10a1c632-b0a5-4d38-b53e-b9a652adc84b"

typedef std::function<void(String)> OnBleIdentify;

class BleFlagDevices : public BLECharacteristicCallbacks {
private:
    BLECharacteristic *characteristicDeviceState;
    BLECharacteristic *characteristicIdentify;
    std::vector<OnBleIdentify> listeners;
    void callListeners(String macAddress);
    void onControlIdentify(BLECharacteristic* characteristic);
public:
    BleFlagDevices(BLEService* service);
    void onWrite(BLECharacteristic* characteristic);
    void onIdentify(OnBleIdentify callback);
    void setValue(String payload);
    void notify();
};

#endif //COCKPITXP_BREAKOUT_BOARD_ADDON_BLEFLAGDEVICES_H