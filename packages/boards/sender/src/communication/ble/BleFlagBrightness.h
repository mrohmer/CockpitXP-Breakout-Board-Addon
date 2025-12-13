//
// Created by mrohmer on 12.12.25.
//

#ifndef COCKPITXP_BREAKOUT_BOARD_ADDON_BLEFLAGBRIGHTNESS_H
#define COCKPITXP_BREAKOUT_BOARD_ADDON_BLEFLAGBRIGHTNESS_H

#include <BLEServer.h>
#include <BLE2902.h>

#define BLE_FLAGS_BRIGHTNESS_SERVICE_UUID "4b8b0562-cb00-4469-948f-a36222dd4805"
#define BLE_FLAGS_BRIGHTNESS_CHARACTERISTICS_UUID "21e1ab05-c437-458e-9d57-ba2071109980"

typedef std::function<void(int)> OnBleFlagBrightnessChange;


class BleFlagBrightness : public BLECharacteristicCallbacks {
private:
    BLECharacteristic *characteristicBrightness;
    std::vector<OnBleFlagBrightnessChange> listeners;
    void callListeners(int brightness);
    void onBrightnessChange(BLECharacteristic* characteristic);
public:
    BleFlagBrightness(BLEServer* server);
    void onWrite(BLECharacteristic* characteristic);
    BleFlagBrightness* onChange(OnBleFlagBrightnessChange callback);
    BleFlagBrightness* setValue(int brightness);
    void notify();
    String getServiceUUID();
};


#endif //COCKPITXP_BREAKOUT_BOARD_ADDON_BLEFLAGBRIGHTNESS_H