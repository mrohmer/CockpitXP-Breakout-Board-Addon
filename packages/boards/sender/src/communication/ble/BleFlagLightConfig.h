//
// Created by mrohmer on 12.12.25.
//

#ifndef COCKPITXP_BREAKOUT_BOARD_ADDON_BLEFLAGCONFIG_H
#define COCKPITXP_BREAKOUT_BOARD_ADDON_BLEFLAGCONFIG_H

#include <BLEServer.h>
#include <BLE2902.h>

#define BLE_FLAGS_LIGHT_CONFIG_SERVICE_UUID "4b8b0562-cb00-4469-948f-a36222dd4805"
#define BLE_FLAGS_LIGHT_CONFIG_BRIGHTNESS_CHARACTERISTICS_UUID "21e1ab05-c437-458e-9d57-ba2071109980"
#define BLE_FLAGS_LIGHT_CONFIG_CHAOS_STYLE_CHARACTERISTICS_UUID "5a5c31d5-bf66-450a-91b2-52915bfdbe5c"

typedef std::function<void(int)> OnBleFlagLightConfigBrightnessChange;
typedef std::function<void(String)> OnBleFlagLightConfigChaosStyleChange;


class BleFlagLightConfig : public BLECharacteristicCallbacks {
private:
    BLECharacteristic *characteristicBrightness;
    BLECharacteristic *characteristicChaosStyle;
    std::vector<OnBleFlagLightConfigBrightnessChange> brightnessListeners;
    std::vector<OnBleFlagLightConfigChaosStyleChange> chaosStyleListeners;
    void callBrightnessListeners(int brightness);
    void callChaosStyleListeners(String style);
    void onBrightnessChangeBleEvent(BLECharacteristic* characteristic);
    void onChaosStyleChangeBleEvent(BLECharacteristic* characteristic);
public:
    BleFlagLightConfig(BLEServer* server);
    void onWrite(BLECharacteristic* characteristic);
    BleFlagLightConfig* onBrightnessChange(OnBleFlagLightConfigBrightnessChange callback);
    BleFlagLightConfig* onChaosStyleChange(OnBleFlagLightConfigChaosStyleChange callback);
    BleFlagLightConfig* setBrightness(int brightness);
    BleFlagLightConfig* setChaosStyle(String payload);
    void notify();
    String getServiceUUID();
};


#endif //COCKPITXP_BREAKOUT_BOARD_ADDON_BLEFLAGCONFIG_H