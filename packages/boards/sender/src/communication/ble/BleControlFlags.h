//
// Created by mrohmer on 29.11.25.
//

#ifndef COCKPITXP_BREAKOUT_BOARD_ADDON_BLECONTROL_FLAGS_H
#define COCKPITXP_BREAKOUT_BOARD_ADDON_BLECONTROL_FLAGS_H

#include <BLEServer.h>
#include <BLE2902.h>
#include <models/State.h>

#define BLE_CONTROL_STATE_CHARACTERISTICS_UUID "bf191dbf-5147-440e-96d4-0f8b2080f8ce"

typedef std::function<void(bool enabled, State)> OnBleControlInputChange;

class BleControlFlags : public BLECharacteristicCallbacks {
private:
    BLECharacteristic *characteristic;
    std::vector<OnBleControlInputChange> listeners;
    void callListeners(bool enabled, State state);
public:
    BleControlFlags(BLEService* service);
    void onWrite(BLECharacteristic* characteristic);
    void onChange(OnBleControlInputChange onChange);
};


#endif //COCKPITXP_BREAKOUT_BOARD_ADDON_BLECONTROL_FLAGS_H