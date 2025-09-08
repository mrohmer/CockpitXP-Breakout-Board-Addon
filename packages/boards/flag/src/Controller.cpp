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

    this->initTicker = new CountingTicker(25, std::bind(&Controller::initAnimationTick, this, std::placeholders::_1));
    return this->now->init();
}
void Controller::initAnimationTick(int count) {
    int steps = 70;
    int v = std::abs((count + steps / 2) % steps - steps / 2) + 5;
    for (auto & element : this->flags) {
        element
            ->clear()
            ->setColor(0, v, v, v)
            ->setColor(1, v, v, v)
            ->setColor(2, v, v, v)
            ->setColor(3, v, v, v)
            ->show();
    }
}
void Controller::onReceiveData(String data) {
    if (this->initTicker != nullptr) {
        this->initTicker->off();
        this->initTicker = nullptr;
    }
    JsonDocument doc;
    deserializeJson(doc, data);
    JsonArray array = doc.as<JsonArray>();

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
Controller* Controller::addFlag(Flags* flag) {
    this->flags.insert(this->flags.end(), flag);
    return this;
}
void Controller::initFlags() {
    for (auto & element : this->flags) {
        element->init();
    }
}