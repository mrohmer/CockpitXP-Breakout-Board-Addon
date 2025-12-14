//
// Created by mrohmer on 29.11.25.
//

#ifndef COCKPITXP_BREAKOUT_BOARD_ADDON_BLECONTROL_FLAGS_H
#define COCKPITXP_BREAKOUT_BOARD_ADDON_BLECONTROL_FLAGS_H

#include <BLEServer.h>
#include <BLE2902.h>
#include <models/State.h>

#define BLE_FLAGS_SERVICE_UUID "d00d3b3f-0ba9-493b-93b8-37c268f7315f"
#define BLE_FLAGS_CONTROL_ENABLE_CHARACTERISTICS_UUID "bf191dbf-5147-440e-96d4-0f8b2080f8ce"
#define BLE_FLAGS_CONTROL_VALUE_CHARACTERISTICS_UUID "010081b9-b828-4f92-ac11-a159ce55ead2"

typedef std::function<void(bool enabled, State)> OnBleControlInputChange;

class BleControlFlags : public BLECharacteristicCallbacks {
private:
    BLECharacteristic *characteristicEnabled;
    BLECharacteristic *characteristicValue;
    std::vector<OnBleControlInputChange> listeners;
    bool enabled = false;
    State state = createState(0, false);
    void callListeners(bool enabled, State state);
    void onControlEnabledChange(BLECharacteristic* characteristic);
    void onControlValueChange(BLECharacteristic* characteristic);
public:
    BleControlFlags(BLEServer* server);
    void onWrite(BLECharacteristic* characteristic);
    void onChange(OnBleControlInputChange onChange);
    void notify();
    String getServiceUUID();
};


#endif //COCKPITXP_BREAKOUT_BOARD_ADDON_BLECONTROL_FLAGS_H