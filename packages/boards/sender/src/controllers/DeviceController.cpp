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

    this->ble->getFlagDevices()->onIdentify(std::bind(&DeviceController::onReceiveIdentify, this, std::placeholders::_1));

    ticker.attach_ms(1000, std::bind(&DeviceController::tick, this));
}
void DeviceController::onReceiveData(uint8_t* macAddress, String payload) {
    JsonDocument doc;
    deserializeJson(doc, payload);

    JsonObject object = doc.as<JsonObject>();

    if (object.isNull() || !object.containsKey("t")) {
        // not an object with a key
        Serial.println("rejecting message");
        return;
    }

    if (object["t"] == "p") {
        this->onReceivePing(macAddress, object["d"]);
    }
}
void DeviceController::onReceivePing(uint8_t* mac, JsonObject payload) {
    long lastPing = millis();
    double batterPercentage = payload.isNull() || payload["b"].isNull() ? 0.0f : payload["b"].as<double>();
    char buff[18]; // "AA:BB:CC:DD:EE:FF" + null terminator
    sprintf(buff, "%02X%02X%02X%02X%02X%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    String macAddress = String(buff);

    if (this->flagDevices.find(macAddress) == this->flagDevices.end()) {
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
void DeviceController::onReceiveIdentify(String macAddress) {
    Serial.printf("Sending identify message to %s\n", macAddress.c_str());
    bool success = this->now->send(macAddress, "{\"t\": \"i\"}");
    if (success) {
        Serial.println("Identify message sent successfully");
    } else {
        Serial.println("Identify message could not be sent");
    }
}
void DeviceController::tick() {
    if ((millis() - this->lastPublish) > 900) {
        this->publishToBle();
    }
}
void DeviceController::publishToBle() {
    this->lastPublish = millis();
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
    long sinceLastPing = millis() - device.lastPing;
    int percentage = std::round(device.batteryPercentage);
    percentage = std::min(std::max(percentage, 0), 100);
    bool online = sinceLastPing <= 2900;
    bool heartbeatMissed = sinceLastPing <= 10000;
    int onlineChar = online + heartbeatMissed;
    return device.macAddress + String(onlineChar) + String(percentage);
}