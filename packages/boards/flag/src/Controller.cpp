//
// Created by kali on 9/2/25.
//

#include "Controller.h"

Controller::Controller() {
    this->now = new FlagNow();
    this->now->onReceive(std::bind(&Controller::onReceiveData, this, std::placeholders::_1));
}
bool Controller::init() {
    this->initFlags();

    this->tickBattery(0);
    this->batteryTicker = new CountingTicker(1, std::bind(&Controller::tickBattery, this, std::placeholders::_1));
    return this->now->init();
}
void Controller::tickBattery(int count) {
    this->forceShowBattery = count < 5;

    double percentage = this->getBatteryPercentage();
    this->showBatteryTimerUpdate(percentage);
    if (percentage >= 0) {
        this->now->setPingDataDouble("b", std::round(percentage * 10000) / 100);
    }
}
void Controller::showBatteryTimerUpdate(double percentage) {
    if (this->dataReceived) {
        return;
    }
    double r = 255.0f * (1.0f - percentage);
    double g = 255.0f * percentage;
    double b = 0;

    for (auto & element : this->flags) {
        element
            ->clear()
            ->setColor(0, r, g, b, 20)
            ->show();
    }
}
double Controller::getBatteryPercentage() {
    if (this->battery == nullptr) {
        return 0;
    }

    return this->battery->getPercentage() / 100;
}
void Controller::onReceiveData(String data) {
    if (this->forceShowBattery) {
        return;
    }

    JsonDocument doc;
    deserializeJson(doc, data);

    JsonObject object = doc.as<JsonObject>();

    if (object.isNull() || !object.containsKey("t")) {
        // not an object with a key
        return;
    }

    if (object["t"] == "c") {
        JsonArray array = object["d"].as<JsonArray>();

        return this->onReceiveColor(array[0], array[1], array[2], array[3]);
    }
    if (object["t"] == "i") {
        return this->onReceiveIdentify();
    }
}
void Controller::onReceiveColor(String c1, String c2, String c3, String c4) {
    this->dataReceived = true;

    this->color1 = c1;
    this->color2 = c2;
    this->color3 = c3;
    this->color4 = c4;

    if (this->identifyTicker != nullptr) {
        return;
    }
    this->updateColor();
}
void Controller::onReceiveIdentify() {
    Serial.println("Received Identify");
    if (this->identifyTicker != nullptr) {
        return;
    }

    this->identifyTicker = new CountingTicker(12, std::bind(&Controller::tickIdentify, this, std::placeholders::_1));
}
void Controller::tickIdentify(int count) {
    if (count > 24) {
        this->identifyTicker->off();
        this->identifyTicker = nullptr;
        this->updateColor();
        return;
    }

    String on = "0082fc22";
    String off = "000";
    String color1 = count % 4 == 0 ? on : off;
    String color2 = count % 4 == 1 ? on : off;
    String color3 = count % 4 == 2 ? on : off;
    String color4 = count % 4 == 3 ? on : off;
    this->showColorOnFlags(color1, color2, color3, color4);
}
void Controller::updateColor() {
    this->showColorOnFlags(this->color1, this->color2, this->color3, this->color4);
}
void Controller::showColorOnFlags(String c1, String c2, String c3, String c4) {
    for (auto & element : this->flags) {
        element
            ->clear()
            ->setColorString(0, c1)
            ->setColorString(1, c2)
            ->setColorString(2, c3)
            ->setColorString(3, c4)
            ->show();
    }
}
Controller* Controller::addFlag(Flags* flag) {
    this->flags.insert(this->flags.end(), flag);
    return this;
}
void Controller::initFlags() {
    for (auto & element : this->flags) {
        element->init();
    }
}
Controller* Controller::setBattery(Battery* battery) {
    this->battery = battery;
    return this;
}
