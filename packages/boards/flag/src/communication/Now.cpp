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

    ticker.attach_ms(1250, std::bind(&Now::sendPing, this));

    return true;
}
void Now::onReceiveData(EspNowRecvInfo *macAddr, Data *data, Length len) {
    this->senderMacAddr = macAddr->src_addr;

    char* buff = (char*) data;
    String buffStr = String(buff);
    this->onReceiveCallback(buffStr);
}
void Now::sendPing() {
    JsonDocument doc;
    doc["t"] = "p";
    doc["d"] = this->pingData;

    String json;
    serializeJson(doc, json);

    this->send(json);
}
Now* Now::onReceive(OnReceiveCallback callback) {
    this->onReceiveCallback = callback;
    return this;
}
bool Now::initPeer() {
    // clear peer data
    memset(&peer, 0, sizeof(peer));
    memcpy(peer.peer_addr, this->senderMacAddr, 6);
    peer.channel = 0;
    peer.encrypt = 0;

    Serial.print("Peer Status: ");
    const uint8_t *peer_addr = this->peer.peer_addr;
    // check if the peer exists
    bool exists = esp_now_is_peer_exist(peer_addr);
    if (exists) {
        // Peer already paired.
        Serial.println("Already Paired");
        return true;
    }

    // Peer not paired, attempt pair
    return this->pairPeer();
}
bool Now::pairPeer() {
    esp_err_t addStatus = esp_now_add_peer(&this->peer);
    if (addStatus == ESP_OK) {
        // Pair success
        Serial.println("Pair success");
        return true;
    }
    else if (addStatus == ESP_ERR_ESPNOW_NOT_INIT) {
        // How did we get so far!!
        Serial.println("ESPNOW Not Init");
        return false;
    }
    else if (addStatus == ESP_ERR_ESPNOW_ARG) {
        Serial.println("Invalid Argument");
        return false;
    }
    else if (addStatus == ESP_ERR_ESPNOW_FULL) {
        Serial.println("Peer list full");
        return false;
    }
    else if (addStatus == ESP_ERR_ESPNOW_NO_MEM) {
        Serial.println("Out of memory");
        return false;
    }
    else if (addStatus == ESP_ERR_ESPNOW_EXIST) {
        Serial.println("Peer Exists");
        return true;
    }
    else {
        Serial.println("Not sure what happened");
        return false;
    }
}
void Now::send(String payload) {
    if (this->senderMacAddr == nullptr) {
        Serial.println("No sender MAC address");
        return;
    }

    if (!this->initPeer()) {
        Serial.println("Failed to init peer");
        return;
    }
    const uint8_t *peer_addr = this->senderMacAddr;

    Serial.print("Sending: "); Serial.println(payload);
    esp_err_t result = esp_now_send(peer_addr, (uint8_t*)payload.c_str(), payload.length());
    Serial.print("Send Status: ");
    if (result == ESP_OK) {
        Serial.println("Success");
    }
    else if (result == ESP_ERR_ESPNOW_NOT_INIT) {
        // How did we get so far!!
        Serial.println("ESPNOW not Init.");
    }
    else if (result == ESP_ERR_ESPNOW_ARG) {
        Serial.println("Invalid Argument");
    }
    else if (result == ESP_ERR_ESPNOW_INTERNAL) {
        Serial.println("Internal Error");
    }
    else if (result == ESP_ERR_ESPNOW_NO_MEM) {
        Serial.println("ESP_ERR_ESPNOW_NO_MEM");
    }
    else if (result == ESP_ERR_ESPNOW_NOT_FOUND) {
        Serial.println("Peer not found.");
    }
    else {
        Serial.println("Not sure what happened");
    }
}
void Now::setPingDataString(String key, String value) {
    this->pingData[key] = value;
}
void Now::setPingDataFloat(String key, float value) {
    this->pingData[key] = value;
}
void Now::setPingDataDouble(String key, double value) {
    this->pingData[key] = value;
}
void Now::setPingDataInt(String key, int value) {
    this->pingData[key] = value;
}
void Now::setPingDataBool(String key, bool value) {
    this->pingData[key] = value;
}