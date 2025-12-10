//
// Created by kali on 9/1/25.
//

#ifndef FLAG_NOW_H
#define NFLAG_OW_H

#include <Now.h>
#include <Ticker.h>
#include "ArduinoJson.h"

typedef std::function<void(String)> OnFlagNoweceiveCallback;


class FlagNow {
private:
    Now* now = Now::getInstance();
    String senderMacAddr;
    OnFlagNoweceiveCallback onReceiveCallback;
    bool canSend = false;
    Ticker ticker;
    JsonDocument pingData;

    void onReceiveData(uint8_t* macAddress, String payload);
    bool initPeer();
    bool pairPeer();
    void sendPing();
public:
    bool init();
    FlagNow* onReceive(OnFlagNoweceiveCallback callback);
    void setPingDataBool(String key, bool value);
    void setPingDataInt(String key, int value);
    void setPingDataFloat(String key, float value);
    void setPingDataDouble(String key, double value);
    void setPingDataString(String key, String value);
};

#endif //FLAG_NOW_H
