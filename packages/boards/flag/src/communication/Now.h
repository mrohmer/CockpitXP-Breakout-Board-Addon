//
// Created by kali on 9/1/25.
//

#ifndef NOW_H
#define NOW_H

#ifdef ESP32
  #include <WiFi.h>
  #include <esp_wifi.h>
  #include <esp_now.h>
typedef const esp_now_recv_info EspNowRecvInfo;
typedef const uint8_t Data;
typedef int Length;
#else
  #include <ESP8266WiFi.h>
  #include <espnow.h>
  #define ESP_OK 0

typedef unsigned char EspNowRecvInfo;
typedef uint8_t Data;
typedef unsigned char Length;
#endif

#include <Ticker.h>
#include "ArduinoJson.h"

typedef std::function<void(String)> OnReceiveCallback;

class Now {
private:
    static Now* instance;
	bool initialised = false;
    OnReceiveCallback onReceiveCallback;
    esp_now_peer_info_t peer;
    uint8_t *senderMacAddr = nullptr;
    bool canSend = false;
    Ticker ticker;
    JsonDocument pingData;

    static void staticOnReceiveData(EspNowRecvInfo *macAddr, Data *data, Length len) {
        Serial.println("Now::staticOnReceiveData");
        if (instance) {
            instance->onReceiveData(macAddr, data, len);
        }
    }
    void onReceiveData(EspNowRecvInfo *macAddr, Data *data, Length len);
    bool initPeer();
    bool pairPeer();
    void sendPing();
    void send(String payload);
public:
    static Now* getInstance() {
        if (!instance) {
            instance = new Now();
        }
        return instance;
    }
    bool init();
    Now* onReceive(OnReceiveCallback callback);
    void setPingDataBool(String key, bool value);
    void setPingDataInt(String key, int value);
    void setPingDataFloat(String key, float value);
    void setPingDataDouble(String key, double value);
    void setPingDataString(String key, String value);
};

#endif //NOW_H
