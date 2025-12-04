//
// Created by mrohmer on 29.11.25.
//

#ifndef COCKPITXP_BREAKOUT_BOARD_ADDON_BLE_H
#define COCKPITXP_BREAKOUT_BOARD_ADDON_BLE_H

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <communication/ble/BleControlFlags.h>

#define BLE_SERVICE_UUID "674b98c9-f0d7-434f-9ab2-1266a0655abc"
#define BLE_IDENTIFY_CHARACTERISTICS_UUID "d70b2c44-b1d9-4fe1-99a7-f20af423c4e7"

typedef std::function<void(bool)> BleOnConnectionChangeCallback;

class Ble : public BLEServerCallbacks {
private:
    BLEServer* server;
    BLEService* service;
    BleControlFlags* controlFlags;
    std::vector<BleOnConnectionChangeCallback> onConnectionChangeListeners;
    bool connected = false;
    void callOnConnectionChangeListeners(bool state);
public:
    void init();
    void onConnect(BLEServer *server);
    void onDisconnect(BLEServer *server);
    void onConnectionChange(BleOnConnectionChangeCallback callback);
    BleControlFlags* getControlFlags();
};


#endif //COCKPITXP_BREAKOUT_BOARD_ADDON_BLE_H