//
// Created by kali on 9/2/25.
//

#include "Controller.h"

Controller::Controller(Flags* flags) {
    this->flags = flags;
    this->now = Now::getInstance()
        ->onReceive(std::bind(&Controller::onReceiveData, this, std::placeholders::_1));
}
bool Controller::init() {
    this->flags
        ->init();

    this->initTicker = new CountingTicker(25, std::bind(&Controller::initAnimationTick, this, std::placeholders::_1));
    return this->now->init();
}
void Controller::initAnimationTick(int count) {
    int steps = 70;
    int v = std::abs((count + steps / 2) % steps - steps / 2) + 5;
    this->flags
        ->setColor(0, v, v, v)
        ->setColor(1, v, v, v)
        ->setColor(2, v, v, v)
        ->setColor(3, v, v, v)
        ->show();
}
void Controller::onReceiveData(String data) {
    if (this->initTicker != nullptr) {
        this->initTicker->off();
        this->initTicker = nullptr;
    }
    JsonDocument doc;
    deserializeJson(doc, data);
    JsonArray array = doc.as<JsonArray>();

    this->flags
        ->clear()
        ->setColorString(0, array[0])
        ->setColorString(1, array[1])
        ->setColorString(2, array[2])
        ->setColorString(3, array[3])
        ->show();
}