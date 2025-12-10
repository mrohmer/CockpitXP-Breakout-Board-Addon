//
// Created by kali on 9/1/25.
//

#include "FlagNow.h"


bool FlagNow::init() {
	WiFi.mode(WIFI_STA);

    if (!this->now->init()) {
        return false;
    }

    this->now->onReceive(std::bind(&FlagNow::onReceiveData, this, std::placeholders::_1, std::placeholders::_2));

    ticker.attach_ms(1250, std::bind(&FlagNow::sendPing, this));

    return true;
}
void FlagNow::sendPing() {
    if (this->senderMacAddr == "") {
        return;
    }

    JsonDocument doc;
    doc["t"] = "p";
    doc["d"] = this->pingData;

    String json;
    serializeJson(doc, json);

    this->now->send(this->senderMacAddr, json);
}
FlagNow* FlagNow::onReceive(OnFlagNoweceiveCallback callback) {
    this->onReceiveCallback = callback;
    return this;
}
void FlagNow::onReceiveData(uint8_t* macAddress, String payload) {
    char buff[18]; // "AA:BB:CC:DD:EE:FF" + null terminator
    sprintf(
        buff,
        "%02X%02X%02X%02X%02X%02X",
        macAddress[0],
        macAddress[1],
        macAddress[2],
        macAddress[3],
        macAddress[4],
        macAddress[5]
    );
    this->senderMacAddr = String(buff);

    this->onReceiveCallback(payload);
}
void FlagNow::setPingDataString(String key, String value) {
    this->pingData[key] = value;
}
void FlagNow::setPingDataFloat(String key, float value) {
    this->pingData[key] = value;
}
void FlagNow::setPingDataDouble(String key, double value) {
    this->pingData[key] = value;
}
void FlagNow::setPingDataInt(String key, int value) {
    this->pingData[key] = value;
}
void FlagNow::setPingDataBool(String key, bool value) {
    this->pingData[key] = value;
}