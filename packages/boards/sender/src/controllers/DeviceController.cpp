//
// Created by mrohmer on 08.12.25.
//

#include "DeviceController.h"

DeviceController::DeviceController(Now* now, Ble* ble) {
    this->ble = ble;
    this->now = now;
}
void DeviceController::init() {
    this->now->onReceive(std::bind(&DeviceController::onReceiveData, this, std::placeholders::_1, std::placeholders::_2));
}
void DeviceController::onReceiveData(uint8_t* macAddress, String payload) {
    JsonDocument doc;
    deserializeJson(doc, payload);

    JsonObject object = doc.as<JsonObject>();

    if (object.isNull() || !object.containsKey("type")) {
        // not an object with a key
        return;
    }

    if (object["type"] == "ping") {
        this->onReceivePing(macAddress, object["data"]);
    }
}
void DeviceController::onReceivePing(uint8_t* mac, JsonObject payload) {
    long lastPing = millis();
    double batterPercentage = payload["battery"].as<double>();
    char buff[18]; // "AA:BB:CC:DD:EE:FF" + null terminator
    sprintf(buff, "%02X%02X%02X%02X%02X%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    String macAddress = String(buff);

    if (this->flagDevices.find("f") == this->flagDevices.end()) {
        this->flagDevices[macAddress] = {
            .macAddress = macAddress,
            .lastPing = lastPing,
            .batteryPercentage = batterPercentage
        };
    } else {
        this->flagDevices[macAddress].lastPing = lastPing;
        this->flagDevices[macAddress].batteryPercentage = batterPercentage;
    }

    for (const auto& pair : this->flagDevices) {
        Serial.printf("flag %s: %d %f\n", pair.first.c_str(), millis() - pair.second.lastPing, pair.second.batteryPercentage);
    }

    this->publishToBle();
}
void DeviceController::publishToBle() {
    String payload = "";
    for (const auto& pair : this->flagDevices) {
        if (payload.length() > 0) {
            payload += ";";
        }
        payload += this->serialiseDevice(pair.second);
    }

    this->ble->getFlagDevices()->setValue(payload);
}
String DeviceController::serialiseDevice(FlagDevice device) {
    bool online = (millis() - device.lastPing) <= 20000;
    int percentage = std::round(device.batteryPercentage);
    return device.macAddress + (online ? "1" : "0") + String(percentage);
}