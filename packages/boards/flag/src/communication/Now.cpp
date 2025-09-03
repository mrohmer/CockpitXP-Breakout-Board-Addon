//
// Created by kali on 9/1/25.
//

#include "Now.h"

Now* Now::instance = nullptr;

bool Now::init() {
	WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK) {
        Serial.println("ESPNow Init Failed");
        return false;
    }

    esp_now_register_recv_cb(staticOnReceiveData);

    return true;
}
void Now::onReceiveData(EspNowRecvInfo *macAddr, Data *data, Length len) {
    char* buff = (char*) data;
    String buffStr = String(buff);
    this->onReceiveCallback(buffStr);
}
Now* Now::onReceive(OnReceiveCallback callback) {
    this->onReceiveCallback = callback;
    return this;
}