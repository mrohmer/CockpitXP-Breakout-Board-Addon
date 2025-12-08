//
// Created by kali on 9/2/25.
//

#include "Controller.h"

Controller::Controller() {
    this->now = Now::getInstance()
        ->onReceive(std::bind(&Controller::onReceiveData, this, std::placeholders::_1));
}
bool Controller::init() {
    this->initFlags();

    this->tickBattery(0);
    this->batteryTicker = new CountingTicker(1000, std::bind(&Controller::tickBattery, this, std::placeholders::_1));
    return this->now->init();
}
void Controller::tickBattery(int count) {
    this->forceShowBattery = count < 5;

    double percentage = this->getBatteryPercentage();
    this->showBatteryTimerUpdate(percentage);
    if (percentage > 0) {
        this->now->setPingDataDouble("b", percentage * 100);
    }
}
void Controller::showBatteryTimerUpdate(double percentage) {
    if (this->dataReceived) {
        return;
    }
    double r = 255.0 * (1.0 - percentage) / 16;
    double g = 255.0 * percentage / 16;
    double b = 0;
    for (auto & element : this->flags) {
        element
            ->clear()
            ->setColor(1, r, g, b, 1)
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

    if (object["t"]== "c") {
        this->dataReceived = true;

        JsonArray array = object["d"].as<JsonArray>();

        for (auto & element : this->flags) {
            element
                ->clear()
                ->setColorString(0, array[0])
                ->setColorString(1, array[1])
                ->setColorString(2, array[2])
                ->setColorString(3, array[3])
                ->show();
        }
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
