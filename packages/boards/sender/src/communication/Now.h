//
// Created by kali on 9/1/25.
//

#ifndef NOW_H
#define NOW_H

#include <vector>

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

typedef std::function<void(uint8_t*, String)> OnReceiveCallback;

class Now {
private:
    static Now* instance;
	bool initialised = false;
    esp_now_peer_info_t broadcastPeer;
    std::vector<OnReceiveCallback> onReceiveCallbacks;
    static void staticOnReceiveData(EspNowRecvInfo *macAddr, Data *data, Length len) {
        Serial.println("Now::staticOnReceiveData");
        if (instance) {
            instance->onReceiveData(macAddr, data, len);
        }
    }

    void onReceiveData(EspNowRecvInfo *macAddr, Data *data, Length len);
    bool initBroadcastPeer();
    bool initPeer(esp_now_peer_info_t peer);
    bool pairPeer(esp_now_peer_info_t peer);
    bool sendWithPeer(esp_now_peer_info_t peer, String payload);
    bool macStringToBytes(const String &macStr, uint8_t mac[6]);
public:
    static Now* getInstance() {
        if (!instance) {
            instance = new Now();
        }
        return instance;
    }
    Now* onReceive(OnReceiveCallback callback);
    bool init();
    bool sendBroadcast(String payload);
    bool send(String macAdress, String payload);
};

#endif //NOW_H
