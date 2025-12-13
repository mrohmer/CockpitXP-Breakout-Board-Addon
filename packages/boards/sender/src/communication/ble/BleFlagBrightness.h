//
// Created by mrohmer on 12.12.25.
//

#ifndef COCKPITXP_BREAKOUT_BOARD_ADDON_BLEFLAGBRIGHTNESS_H
#define COCKPITXP_BREAKOUT_BOARD_ADDON_BLEFLAGBRIGHTNESS_H

#include <BLEServer.h>
#include <BLE2902.h>

#define BLE_FLAGS_BRIGHTNESS_CHARACTERISTICS_UUID "bf191dbf-5147-440e-96d4-0f8b2080f8ce"

typedef std::function<void(int)> OnBleFlagBrightnessChange;


class BleFlagBrightness : public BLECharacteristicCallbacks {
private:
    BLECharacteristic *characteristicBrightness;
    std::vector<OnBleFlagBrightnessChange> listeners;
    void callListeners(int brightness);
    void onBrightnessChange(BLECharacteristic* characteristic);
public:
    BleFlagBrightness(BLEService* service);
    void onWrite(BLECharacteristic* characteristic);
    BleFlagBrightness* onChange(OnBleFlagBrightnessChange callback);
    BleFlagBrightness* setValue(int brightness);
    void notify();
};


#endif //COCKPITXP_BREAKOUT_BOARD_ADDON_BLEFLAGBRIGHTNESS_H