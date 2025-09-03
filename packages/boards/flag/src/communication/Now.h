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

typedef std::function<void(String)> OnReceiveCallback;

class Now {
private:
    static Now* instance;
	bool initialised = false;
    OnReceiveCallback onReceiveCallback;

    static void staticOnReceiveData(EspNowRecvInfo *macAddr, Data *data, Length len) {
        Serial.println("Now::staticOnReceiveData");
        if (instance) {
            instance->onReceiveData(macAddr, data, len);
        }
    }
    void onReceiveData(EspNowRecvInfo *macAddr, Data *data, Length len);

public:
    static Now* getInstance() {
        if (!instance) {
            instance = new Now();
        }
        return instance;
    }
    bool init();
    Now* onReceive(OnReceiveCallback callback);
};

#endif //NOW_H
